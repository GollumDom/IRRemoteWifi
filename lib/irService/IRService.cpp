#include "IRService.h"

IRService::~IRService() {
    this->clean();
}

void IRService::init(
    LJsonAsyncWebServer* webserver,
    uint32 pinSender,
    uint32 pinReceiver,
    int32 pinLedSend
) {

    // log_n(F("Init IR Uusing IRremote library version " VERSION_IRREMOTE));

    this->clean();

    this->webServer = webServer;
    this->pinSender = pinSender;
    this->pinReceiver = pinReceiver;
    this->pinLedSend = pinLedSend;

    // IrReceiver.begin(this->pinReceiver, DISABLE_LED_FEEDBACK);
    // IrSender.begin(this->pinSender, DISABLE_LED_FEEDBACK);


    log_l("   - Receive IR pin: "); log_n(this->pinReceiver);
    log_l("   - Sender IR pin: "); log_n(this->pinSender);

    if (this->pinLedSend != -1) {
        pinMode(this->pinLedSend, OUTPUT);
        log_l("   - Led IR send pin: "); log_n(this->pinLedSend);
    }

    this->initWebServer();

}

void IRService::loop() {
    // if (IrReceiver.available()) {
    //     this->storeCode();
    // }
}

void IRService::clean() {
    log_n("Clean IR");
    for (AsyncCallbackWebHandler* handler: this->webServerHandlers) {
        this->webServer->removeHandler(handler);
        delete handler;
    }
    this->webServerHandlers.clear();
}

void IRService::initWebServer() {
    log_n("IR init webserver");

    AsyncCallbackWebHandler* handlers[] = {
        
        &this->webServer->on("/api/ir/receive-history", HTTP_GET, [this] (AsyncWebServerRequest* request) {
            request->send(200, "application/json", ljson_stringify((new LJsonObject)
                ->addChild("mode", "")
            , true));
        })

    };
    this->webServerHandlers.push_back(handlers[0]);
}

void IRService::storeCode() {
    
    // IRData* aIRReceivedData = IrReceiver.read();

    // if (aIRReceivedData->flags & IRDATA_FLAGS_IS_REPEAT) {
    //     log_n(F("Ignore repeat"));
    //     return;
    // }
    // if (aIRReceivedData->flags & IRDATA_FLAGS_IS_AUTO_REPEAT) {
    //     log_n(F("Ignore autorepeat"));
    //     return;
    // }
    // if (aIRReceivedData->flags & IRDATA_FLAGS_PARITY_FAILED) {
    //     log_n(F("Ignore parity error"));
    //     return;
    // }

    // SignalData* signalData = new SignalData();
    // /*
    //  * Copy decoded data
    //  */
    // signalData->receivedIRData = *aIRReceivedData;

    // if (signalData->receivedIRData.protocol == UNKNOWN) {
    //     log_l(F("Received unknown code and store "));
    //     log_l(IrReceiver.decodedIRData.rawDataPtr->rawlen - 1);
    //     log_n(F(" timing entries as raw "));
    //     IrReceiver.printIRResultRawFormatted(&Serial, true); // Output the results in RAW format

    //     signalData->rawCodeLength = IrReceiver.decodedIRData.rawDataPtr->rawlen - 1;
    //     /*
    //      * Store the current raw data in a dedicated array for later usage
    //      */
    //     IrReceiver.compensateAndStoreIRResultInArray(signalData->rawCode);
    
    // } else {
    //     IrReceiver.printIRResultShort(&Serial); log_n();
    //     signalData->receivedIRData.flags = 0; // clear flags -esp. repeat- for later sending
    // }

    // this->signalDataHistory.push_back(signalData);

    // IrReceiver.resume(); 
}
