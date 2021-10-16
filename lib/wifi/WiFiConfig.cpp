#include "WiFiConfig.h"

String WiFiConfig::dhcpMode2String(WiFiConfigDHCPMode mode) {
    switch (mode) {
        case DHCP: return "DHCP";
        case DHCP_DNS: return "DHCP_DNS";
        case STATIC: return "STATIC";
        default: return "ERROR";
    }
}

WiFiConfigDHCPMode WiFiConfig::string2DHCPMode(String str) {
    if (str == "DHCP_DNS") return DHCP_DNS;
    if (str == "STATIC") return STATIC;
    return DHCP;
}

WiFiConfig::~WiFiConfig() {
    if (this->ip) delete this->ip;
    if (this->mask) delete this->mask;
    if (this->gateway) delete this->mask;
    if (this->dns1) delete this->dns1;
    if (this->dns2) delete this->dns2;
}

WiFiConfigMode WiFiConfig::string2Mode(String str) {
    if (str == "AP") return AP;
    return STATION;
}

String WiFiConfig::mode2String(WiFiConfigMode mode) {
    switch (mode) {
        case AP: return "AP";
        default: return "STATION";
    }
}

bool WiFiConfig::isValid() {
    return this->ssid != "" && (
        (
            this->mode == STATION && (
                this->dhcp == DHCP ||
                this->dhcp == DHCP_DNS ||
                (
                    this->dhcp == STATIC &&
                    this->ip &&
                    this->mask
                )
            )
        ) ||
        (
            this->mode == AP
        )
    );
}

LJsonNode* WiFiConfig::toJson() {
    return (new LJsonObject())
        ->addChild("ssid"      , this->ssid)
        ->addChild("password"  , this->password)
        ->addChild("mode"      , WiFiConfig::mode2String(this->mode))
        ->addChild("dhcp"      , WiFiConfig::dhcpMode2String(this->dhcp))
        ->addChild("ip"        , this->ip      ? this->ip     ->toString() : "")
        ->addChild("mask"      , this->mask    ? this->mask   ->toString() : "")
        ->addChild("gateway"   , this->gateway ? this->gateway->toString() : "")
        ->addChild("dns1"      , this->dns1    ? this->dns1   ->toString() : "")
        ->addChild("dns2"      , this->dns2    ? this->dns2   ->toString() : "")
    ;
}

bool jsonChildToIp(LJsonObject* obj, String key, IPAddress** ip) {
    if (LJsonScalar* c = obj->getChildNumberString(key)) {
        String s = c->getValueStr();
        
        if (s != "" ) {
            (*ip) = new IPAddress();
            if((*ip)->fromString(s)) {
                delete (*ip);
                (*ip) = NULL;
                return false;
            }
        }
        return true;
    }
    return false;
}

bool WiFiConfig::fromJson(LJsonNode* json) {
    if (!json->isObject()) return false;
    
    LJsonObject* obj = (LJsonObject*)json;
    if (LJsonScalar* c = obj->getChildNumberString("ssid")    ) { this->ssid     = c->getValueStr(); } else return false;
    if (LJsonScalar* c = obj->getChildNumberString("password")) { this->password = c->getValueStr(); } else return false;
    if (LJsonScalar* c = obj->getChildNumberString("mode")    ) { this->mode     = WiFiConfig::string2Mode(c->getValueStr()); } else return false;
    if (LJsonScalar* c = obj->getChildNumberString("dhcp")    ) { this->dhcp     = WiFiConfig::string2DHCPMode(c->getValueStr()); } else return false;
    if (!jsonChildToIp(obj, "ip"     , &this->ip     )) { return false; }
    if (!jsonChildToIp(obj, "mask"   , &this->mask   )) { return false; }
    if (!jsonChildToIp(obj, "gateway", &this->gateway)) { return false; }
    if (!jsonChildToIp(obj, "dns1"   , &this->dns1   )) { return false; }
    if (!jsonChildToIp(obj, "dns2"   , &this->dns2   )) { return false; }

    return this->isValid();
}

