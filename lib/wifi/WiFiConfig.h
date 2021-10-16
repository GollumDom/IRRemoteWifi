#ifndef wifi_WiFiConfig_h
#define wifi_WiFiConfig_h

#include "LJson.h"
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#else //ESP32
    #include <WiFi.h>
#endif

enum WiFiConfigMode {
    AP,
    STATION
};

enum WiFiConfigDHCPMode {
    DHCP,
    DHCP_DNS,
    STATIC
};



class WiFiConfig: public LNormalized, public LDenormalized {
    public:

        virtual ~WiFiConfig();

        static String dhcpMode2String(WiFiConfigDHCPMode mode);
        static WiFiConfigDHCPMode string2DHCPMode(String str);
        static String mode2String(WiFiConfigMode mode);
        static WiFiConfigMode string2Mode(String str);
        
        WiFiConfigMode mode = STATION;
        String ssid = "";
        String password = "";
        WiFiConfigDHCPMode dhcp = DHCP;
        IPAddress* ip = NULL;
        IPAddress* mask = NULL;
        IPAddress* gateway = NULL;
        IPAddress* dns1 = NULL;
        IPAddress* dns2 = NULL;

        bool isValid();
        virtual LJsonNode* toJson();
        virtual bool fromJson(LJsonNode* json);
};

#endif