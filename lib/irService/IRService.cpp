#include "IRService.h"

IRService::IRService() {
    this->webServer = NULL;
    this->pinSender = 0;
    this->pinReceiver = 0;
    this->pinLedSend = -1;
}

IRService::~IRService() {
    this->clean();
}

void IRService::init(
    LJsonAsyncWebServer* webServer,
    FileFS* fileFS,
    uint32 pinSender,
    uint32 pinReceiver,
    int32 pinLedSend
) {
    this->clean();

    log_n(F("Init IR Uusing IRremote library version " VERSION_IRREMOTE ": "));

    this->webServer = webServer;
    this->fileFS = fileFS;
    this->pinSender = pinSender;
    this->pinReceiver = pinReceiver;
    this->pinLedSend = pinLedSend;

    IrReceiver.begin(this->pinReceiver, DISABLE_LED_FEEDBACK);
    IrSender.begin(this->pinSender, DISABLE_LED_FEEDBACK);

    log_l("   - Receive IR pin: "); log_n(this->pinReceiver);
    log_l("   - Sender IR pin: "); log_n(this->pinSender);

    if (this->pinLedSend != -1) {
        pinMode(this->pinLedSend, OUTPUT);
        log_l("   - Led IR send pin: "); log_n(this->pinLedSend);
    }


    if (!LittleFS.exists(STORE_PATH)) {
        LittleFS.mkdir(STORE_PATH);
    }

    this->initWebServer();

}

void IRService::loop() {

    if (IrReceiver.available()) {
        this->storeCode();
    }
    if (this->signalsSend.size()) {
        SignalSend* send = this->signalsSend[0];
        uint64 now = millis();

        if (send->state == IDLE) {
            IrReceiver.stop();
            send->prevouis = now;
            send->state = SEND;
            this->sendSignal(send->value);
        } else
        if (send->state == SEND) {
            if (now - send->prevouis < send->repeat) {          
                this->sendSignal(send->value, true);
            } else {
                send->prevouis = now;
                send->state = WAIT;
            }
        } else
        if (send->state == WAIT) {
            if (now - send->prevouis < send->wait) {
                log_n("Wait after send");
                delay(DELAY_BETWEEN_REPEAT);
            } else {
                log_l("End send signal "); log_n(send->value);
                IrReceiver.start();
                delete send;
                this->signalsSend.erase(this->signalsSend.begin());
            }
        }
    }
}

void IRService::sendSignal(String key, bool repeat) {
    log_l("Send signal "); if (repeat) log_l("(REPEAT) ");  log_n(key);
    
    SignalData* signal = this->findSignalDataByKey(key);
    
    if (signal) {
        
        if (this->pinLedSend != -1) {
            digitalWrite(this->pinLedSend, HIGH);
        }

        if (repeat) {
            signal->receivedIRData.flags = IRDATA_FLAGS_IS_REPEAT;
        }

        if (signal->receivedIRData.protocol == UNKNOWN) { // i.e. raw
            // Assume 38 KHz
            IrSender.sendRaw(signal->rawCode, signal->rawCodeLength, 38);

            log_l(F("Sent raw "));
            log_l(signal->rawCodeLength);
            log_n(F(" marks or spaces"));
        } else {
            IrSender.write(&signal->receivedIRData, NO_REPEATS);

            log_l(F("Sent: "));
            printIRResultShort(&Serial, &signal->receivedIRData);
        }
        
        delete signal;
        delay(DELAY_BETWEEN_REPEAT);

        if (this->pinLedSend != -1) {
            digitalWrite(this->pinLedSend, LOW);
        }

    } else {
        log_l("Signal "); log_l(key); log_n(" not exist");
    }
    
}

void IRService::clean() {
    log_n("Clean IR");
    for (AsyncCallbackWebHandler* handler: this->webServerHandlers) {
        this->webServer->removeHandler(handler);
        delete handler;
    }
    this->webServerHandlers.clear();
    for (SignalSend* signal: this->signalsSend) {
        delete signal;
    }
    this->signalsSend.clear();
}

void IRService::initWebServer() {
    log_n("IR init webserver");

    AsyncCallbackWebHandler* handlers[] = {
        
        &this->webServer->on("/api/ir/history-receive", HTTP_GET, [this] (AsyncWebServerRequest* request) {
            request->send(200, "application/json", ljson_stringify((VectorLNormalize*)&this->signalDataHistory));
        }),

        &this->webServer->onJson<SignalStoreInput>("/api/ir/store", HTTP_POST, [this] (AsyncWebServerRequest* request, SignalStoreInput* signalStoreInput) {

            SignalData* signal = this->findSignalDataInHistoryById(signalStoreInput->id);
            if (!signal) {
                delete signalStoreInput;
                request->send(400, "application/json", "{\"error\": \"Signal not found in history\"}");
                return;
            }

            this->storeSignal(signalStoreInput->key, signal);

            delete signalStoreInput;

            request->send(200, "application/json", ljson_stringify(signal));
        }),

        &this->webServer->onJson<SignalData>("/api/ir/restore", HTTP_POST, [this] (AsyncWebServerRequest* request, SignalData* signal) {

            AsyncWebParameter* key = request->getParam("key");
            if (key) {
                this->storeSignal(key->value(), signal);
                request->send(200, "application/json", ljson_stringify(signal, true));
                return;
            }
            request->send(400, "application/json", "{\"error\":\"You must use key in param\"}");
        }),

        &this->webServer->on("/api/ir/store", HTTP_GET, [this] (AsyncWebServerRequest* request) {
            AsyncWebParameter* key = request->getParam("key");

            if (key) {
                SignalData* signal = this->findSignalDataByKey(key->value());
                if (signal) {
                    request->send(200, "application/json", ljson_stringify(signal, true));
                } else {
                    request->send(404, "application/json", "{\"error\":\"Signal not found\"}");
                }
                return;
            }
            std::vector<String> list = this->listSignalStored();
            request->send(200, "application/json", ljson_stringify(&list));
        }),

        &this->webServer->onJson<SignalsSendInput>("/api/ir/send", HTTP_POST, [this] (AsyncWebServerRequest* request, SignalsSendInput* signalsSend, String body) {
            for (SignalSend* signal : signalsSend->signals) {
                this->signalsSend.push_back(signal->clone());
            }
            request->send(200, "application/json", ljson_stringify(signalsSend, true));
        }),

        &this->webServer->on("/api/ir/delete", HTTP_POST, [this] (AsyncWebServerRequest* request) {
            AsyncWebParameter* key = request->getParam("key");
            if (key) {
                this->deleteSignalDataByKey(key->value());
                request->send(200, "application/json", "{\"success\":true}");
                return;
            }
            request->send(400, "application/json", "{\"error\":\"You must use key in param\"}");
        })

    };
    this->webServerHandlers.push_back(handlers[0]);
    this->webServerHandlers.push_back(handlers[1]);
    this->webServerHandlers.push_back(handlers[2]);
    this->webServerHandlers.push_back(handlers[3]);
    this->webServerHandlers.push_back(handlers[4]);
    this->webServerHandlers.push_back(handlers[5]);

}

std::vector<String> IRService::listSignalStored() {
    std::vector<String> list;
    Dir dir = LittleFS.openDir(STORE_PATH);
    while(dir.next()) {
		list.push_back(dir.fileName());
	}
    return list;
}

void IRService::clearStoredSignals() {
    log_n("Clear stored signal");

    for(String key: this->listSignalStored()) {
        LittleFS.remove(F(STORE_PATH "/") + key);
	}
}

void IRService::storeSignal(String key, SignalData* signal) {
    log_n(F("Store signal data on " STORE_PATH "/")); log_n(key);
    this->fileFS->filePutContent(F(STORE_PATH "/") + key, ljson_stringify(signal));
}

SignalData* IRService::findSignalDataByKey(String key) {
    String path = F(STORE_PATH "/") + key;
    if (LittleFS.exists(path)) {
        String json;
        if (this->fileFS->fileReadContent(path, json)) {
            SignalData* signal = new SignalData();
            if (ljson_parse(json, signal)) {
                return signal;
            }
            delete signal;
        }
    }
    return NULL;
}

void IRService::deleteSignalDataByKey(String key) {
     String path = F(STORE_PATH "/") + key;
    if (LittleFS.exists(path)) {
        LittleFS.remove(path);
    }
}

SignalData* IRService::findSignalDataInHistoryById(int id) {
    for (SignalData* s: this->signalDataHistory) {
        if (s->id == id) {
            return s;
        }
    }
    return NULL;
}

void IRService::storeCode() {
    
    IRData* aIRReceivedData = IrReceiver.read();

    if (aIRReceivedData->flags & IRDATA_FLAGS_IS_REPEAT) {
        log_n(F("Ignore repeat"));
        IrReceiver.resume(); 
        return;
    }
    if (aIRReceivedData->flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
        log_n(F("Ignore autorepeat"));
        IrReceiver.resume(); 
        return;
    }
    if (aIRReceivedData->flags & IRDATA_FLAGS_PARITY_FAILED) {
        log_n(F("Ignore parity error"));
        IrReceiver.resume(); 
        return;
    }

    SignalData* signalData = new SignalData();
    /*
     * Copy decoded data
     */
    signalData->receivedIRData = *aIRReceivedData;

    if (signalData->receivedIRData.protocol == UNKNOWN) {
        log_l(F("Received unknown code and store "));
        log_l(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
        log_n(F(" timing entries as raw "));
        IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format

        signalData->rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
        /*
         * Store the current raw data in a dedicated array for later usage
         */
        IrReceiver.compensateAndStoreIRResultInArray(signalData->rawCode);
    
    } else {
        IrReceiver.printIRResultShort(&Serial); log_n();
        signalData->receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    }

    this->signalDataHistory.push_back(signalData);
    if (this->signalDataHistory.size() > 10) {
         this->signalDataHistory.erase(this->signalDataHistory.begin());
    }

    IrReceiver.resume(); 
}
