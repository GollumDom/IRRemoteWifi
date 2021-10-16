#ifndef WiFiResult_h
#define WiFiResult_h

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <LJson.h>

class WiFiResult : public LNormalized
{
  public:

    static uint32 rssi2Quality(int rssi);
    static String encryptionTypeToString(uint8 encryptionType);

    bool duplicate;
    String SSID;
    uint8 encryptionType;
    int32 RSSI;
    uint8* BSSID;
    int32 channel;
    bool isHidden;

    WiFiResult(uint32 index);
    uint32 getQuality();
    String getEncryptionTypeLabel();

    virtual LJsonObject* toJson();
};

#endif