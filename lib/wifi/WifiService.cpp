#include "WifiService.h"

String mac2Str(const uint8 mac[6]) {
    char macStr[18] = { 0 };
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

WifiService::WifiService() {
    this->previous = millis();
    this->rescueMode = false;
    this->name = "";
    this->webServer = NULL;
    this->pinLedStatus = -1;
    this->scanRunning = false;
    this->scanResults = new WiFiResultList();
    this->config = NULL;
}

WifiService::~WifiService() {
    this->clean();
    delete this->scanResults;
}

String WifiService::getApSSID() {
    String chipID = String(ESP.getChipId(), HEX);
    chipID.toUpperCase();
    return F("ESP8266_") + this->name + F("_") + chipID;
}

String WifiService::getApPassword() {
    String chipID = String(ESP.getChipId(), HEX);
    chipID.toUpperCase();
    return F("pass_") + chipID;
}

void WifiService::init(String name, FileFS* fileFS) {
    this->init(name, fileFS, NULL, -1);
}

void WifiService::init(String name, FileFS* fileFS, int pinLedStatus) {
    this->init(name, fileFS, NULL, pinLedStatus);
}

void WifiService::init(String name, FileFS* fileFS, LJsonAsyncWebServer* webServer) {
    this->init(name, fileFS, webServer, -1);
}

void WifiService::init(String name, FileFS* fileFS, LJsonAsyncWebServer* webServer, int pinLedStatus) {

    log_n("Initialize wifi service");
    log_l("    - MAC Address: "); log_n(WiFi.macAddress());
    
    this->clean();
    this->loadConfig();

    this->name = name;
    this->fileFS = fileFS;
    this->webServer = webServer;
    this->pinLedStatus = pinLedStatus;
    this->fileFS = fileFS;

    if (this->useLed()) {
        pinMode(this->pinLedStatus, OUTPUT);
        digitalWrite(this->pinLedStatus, LOW);
    }

    this->scanAsync();
    this->initWebServer();
    this->startFromConfig();
}

void WifiService::startFromConfig() {
    this->rescueMode = false;

    WiFi.hostname(this->getApSSID());

    if (this->config && this->config->isValid()) {

        if (this->config->mode == AP) { 
            WiFi.mode(WIFI_AP);
            log_n("Start wifi mode AP");
            log_l("    - SSID:     "); log_n(this->config->ssid);
            log_l("    - Password: "); log_n(this->config->password);

            if (this->config->password) {
                WiFi.softAP(this->getApSSID(), this->getApPassword());
            } else {
                WiFi.softAP(this->getApSSID());
            }
        } else {
            WiFi.mode(WIFI_STA);
            log_n("Start wifi mode Station");
            log_l("    - SSID:      "); log_n(this->config->ssid);
            log_l("    - Password:  "); log_n(this->config->password);
            log_l("    - DHCP Mode: "); log_n(WiFiConfig::dhcpMode2String(this->config->dhcp));
            log_l("    - Ip:        "); log_n(this->config->ip      ? this->config->ip     ->toString() : "");
            log_l("    - Mask:      "); log_n(this->config->mask    ? this->config->mask   ->toString() : "");
            log_l("    - Gateway:   "); log_n(this->config->gateway ? this->config->gateway->toString() : "");
            log_l("    - Dns1:      "); log_n(this->config->dns1    ? this->config->dns1   ->toString() : "");
            log_l("    - Dns2:      "); log_n(this->config->dns2    ? this->config->dns2   ->toString() : "");

            WiFi.config(
                this->config->dhcp == STATIC && this->config->ip ? *this->config->ip : IPAddress(),
                this->config->dhcp == STATIC && this->config->mask ? *this->config->mask : IPAddress(),
                this->config->dhcp == STATIC && this->config->gateway ? *this->config->gateway : IPAddress(),
                this->config->dhcp != DHCP ? ( this->config->dns1 ? *this->config->dns1 : (this->config->dns2 ? *this->config->dns2 :IPAddress()) ) : IPAddress(),
                this->config->dhcp != DHCP && this->config->dns1 && this->config->dns2 ? *this->config->dns2 : IPAddress()
            );
            
            if (this->config->password) {
               WiFi.begin(this->config->ssid, this->config->password);
            } else {
                WiFi.begin(this->config->ssid);
            }
        }
    } else {
        this->startRescueMode();
    }
}

void WifiService::startRescueMode() {
    log_n("Start wifi Rescue mode");
    log_l("    - SSID:    "); log_n(this->getApSSID());
    log_l("    - Password: "); log_n(this->getApPassword());
    this->rescueMode = true;
    WiFi.mode(WIFI_STA);
    WiFi.softAP(this->getApSSID(), this->getApPassword());
}

void WifiService::clean() {
    log_n("Clean Wifi Service");
    for (AsyncCallbackWebHandler* handler: this->webServerHandlers) {
        this->webServer->removeHandler(handler);
        delete handler;
    }
    this->webServerHandlers.clear();
    this->clearScanResults();
    if (this->config) {
        delete this->config;
        this->config = NULL;
    }
}

void WifiService::clearScanResults() {
    for (WiFiResult* r: *this->scanResults) {
        delete r;
    }
    this->scanResults->clear();
}

WiFiResultList* WifiService::scanAsync() {
    if (!this->scanRunning) {
        this->scanRunning = true;

        log_n("Wifi Scan start ... ");
        WiFi.scanNetworksAsync([this](int n) {
            this->clearScanResults();
            
            if (n > 0) {
                log_l("Wifi Scan network(s) found ("); log_l(n); log_n("): ");
                for (int i = 0; i < n; i++)
                {
                    WiFiResult* result = new WiFiResult(i); 
                    this->scanResults->push_back(result);
                    log_n("  - " + WiFi.SSID(i));
                }
                log_n();
            } else {
                log_l("Wifi Scan network(s) found (0)");
            }

            std::sort(this->scanResults->begin(), this->scanResults->end(), [](WiFiResult* a, WiFiResult* b) {
                return a->getQuality() > b->getQuality();
            });

            this->scanRunning = false;
        });
    }
    return this->scanResults;
}

void WifiService::initWebServer() {
    if (this->webServer) {
        log_n("Wifi init webserver");

        AsyncCallbackWebHandler* handlers[] = {
            
            &this->webServer->on("/api/wifi/status", HTTP_GET, [this] (AsyncWebServerRequest* request) {
                request->send(200, "application/json", ljson_stringify((new LJsonObject)
                    ->addChild("mode", this->rescueMode ? "RESCUE" : WiFiConfig::mode2String(this->config->mode))
                    ->addChild("ip", WiFi.localIP().toString())
                    ->addChild("quality", this->rescueMode || this->config->mode == AP ? 100 : (int)WiFiResult::rssi2Quality(WiFi.RSSI()))
                , true));
            }),

            &this->webServer->on("/api/wifi/scan", HTTP_GET, [this] (AsyncWebServerRequest* request) {
                WiFiResultList* list = this->scanAsync();
                request->send(200, "application/json", ljson_stringify((VectorLNormalize*) list));
            }),

            &this->webServer->on("/api/wifi/config", HTTP_GET, [this] (AsyncWebServerRequest* request) {
                WiFiConfig defaultConfig;
                defaultConfig.ssid = this->getApSSID();
                WiFiConfig* config = this->config ? this->config : &defaultConfig;
                request->send(200, "application/json", ljson_stringify(config));
            }),

            &this->webServer->onJson<WiFiConfig>("/api/wifi/config", HTTP_POST, [this] (AsyncWebServerRequest* request, WiFiConfig* config) {
                if (this->config) {
                    delete this->config;
                }
                this->config = config;
                this->saveConfig();
                this->restartTime = millis() + 1000;
                request->send(200, "application/json", ljson_stringify(config));
            }),

        };
        this->webServerHandlers.push_back(handlers[0]);
        this->webServerHandlers.push_back(handlers[1]);
        this->webServerHandlers.push_back(handlers[2]);
        this->webServerHandlers.push_back(handlers[3]);

    }
}

void WifiService::saveConfig() {
    String str = ljson_stringify(this->config);
    log_l("Save config: "); log_n(str);
    if (!this->fileFS->filePutContent("/wifi.json", str)) {
        log_n("Error on open wifi.json");
    }
}

void WifiService::loadConfig() {
    String str;
    log_l("Load config: ");
    if (this->fileFS->fileReadContent("/wifi.json", str)) {
        log_n(str);
        LJsonNode* json = ljson_parse(str);
        if (json) {
            if (this->config) {
                delete this->config;
            }
            this->config = new WiFiConfig();
            if(this->config->fromJson(json)) {
                delete json;
                return;
            }
            delete this->config;
            this->config = NULL;
        }
        delete json;
        log_n("Error on parse json on /wifi.json");
    } else {
        log_n("Error on open /wifi.json");
    }
}

void WifiService::clearConfig() {
    log_l("Clear config");
    if (LittleFS.exists("/wifi.json")) {
        LittleFS.remove("/wifi.json");
    }
    if (this->config) {
        delete this->config;
        this->config = NULL;
    }
    
}

bool WifiService::useLed() {
    return this->pinLedStatus != -1;
}

void WifiService::loop() {

    if (this->restartTime != 0 && this->restartTime < millis()) {
        this->restartTime = 0;
        this->startFromConfig();
    }

    if (this->useLed()) {

        if (this->rescueMode) {
            if (millis() - this->previous > 100) {
                this->previous = millis();
                if (digitalRead(this->pinLedStatus) == LOW) {
                    digitalWrite(this->pinLedStatus, HIGH);
                } else {
                    digitalWrite(this->pinLedStatus, LOW);
                }
            }
        } else
        if (WiFi.getMode() == WIFI_AP) {
            digitalWrite(this->pinLedStatus, HIGH);
        } else
        if (WiFi.isConnected()) {
            digitalWrite(this->pinLedStatus, HIGH);
        } else {
            if (millis() - this->previous > 700) {
                this->previous = millis();
                if (digitalRead(this->pinLedStatus) == LOW) {
                    digitalWrite(this->pinLedStatus, HIGH);
                } else {
                    digitalWrite(this->pinLedStatus, LOW);
                }
            }
        }
    }
    

}
