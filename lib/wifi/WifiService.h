#ifndef WifiService_h
#define WifiService_h

#include <Arduino.h>
#include <helper.h>
#include <exception>
#include <vector>
#include <LJsonAsyncWebServer.h>
#include <FileFS.h>
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#else //ESP32
    #include <WiFi.h>
#endif
#include "WiFiResult.h"
#include "WiFiConfig.h"

#define WiFiResultList std::vector<WiFiResult*>

class WifiService {
    public:
        WifiService();
        ~WifiService();
        String getApSSID();
        String getApPassword();
        void init(String name, FileFS* fileFS);
        void init(String name, FileFS* fileFS, LJsonAsyncWebServer* webserver);
        void init(String name, FileFS* fileFS, int pinLedStatus);
        void init(String name, FileFS* fileFS, LJsonAsyncWebServer* webserver, int pinLedStatus);
        void loop();
        void startRescueMode();
        bool useLed();
        WiFiResultList* scanAsync();

        void saveConfig();
        void loadConfig();
        void clearConfig();

    private:

        static bool eventIsInit;
        static void initEvent();

        String name;
        FileFS* fileFS;
        LJsonAsyncWebServer* webServer;
        int pinLedStatus;
        uint64 previous;
        bool rescueMode;
        std::vector<AsyncCallbackWebHandler*> webServerHandlers;
        WiFiResultList* scanResults;
        bool scanRunning;
        WiFiConfig* config;
        uint64 restartTime = 0;

        void clearScanResults();
        void clean();
        void initWebServer();
        void startFromConfig();
};

#endif