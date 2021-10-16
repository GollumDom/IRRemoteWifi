#include "WiFiResult.h"

uint32 WiFiResult::rssi2Quality(int RSSI) {
    int quality = 0;

    if (RSSI <= -100) {
        quality = 0;
    }
    else if (RSSI >= -50) {
        quality = 100;
    } else {
        quality = 2 * (RSSI + 100);
    }

    return quality;
}

String WiFiResult::encryptionTypeToString(uint8 encryptionType) {
    switch (encryptionType) {
        case ENC_TYPE_NONE: return "NONE";
        case ENC_TYPE_WEP: return "WEP";
        case ENC_TYPE_TKIP: return "WPA";
        case ENC_TYPE_CCMP: return "WPA2";
        case ENC_TYPE_AUTO: return "WPA_WPA2";
        default: return "UNKNOW";
    }
}

WiFiResult::WiFiResult(uint32 index) {
    WiFi.getNetworkInfo(
        index,
        this->SSID,
        this->encryptionType,
        this->RSSI,
        this->BSSID,
        this->channel,
        this->isHidden
    );
}

uint32 WiFiResult::getQuality() {
    return WiFiResult::rssi2Quality(this->RSSI);
}

String WiFiResult::getEncryptionTypeLabel() {
    return WiFiResult::encryptionTypeToString(this->encryptionType);
}

LJsonObject* WiFiResult::toJson() {
    return (new LJsonObject())
        ->addChild("ssid", this->SSID)
        ->addChild("bssid", (int)this->BSSID)
        ->addChild("encryption_type", this->getEncryptionTypeLabel())
        ->addChild("quality", (long)this->getQuality())
        ->addChild("channel", this->channel)
        ->addChild("is_hidden", this->isHidden)
    ;
}