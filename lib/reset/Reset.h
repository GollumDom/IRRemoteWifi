#ifndef reset_Reset_h
#define reset_Reset_h

#include <Arduino.h>
#include <helper.h>
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#else //ESP32
    #include <WiFi.h>
#endif

class Reset {
    public:
        Reset();
        ~Reset();
        void init(
            u32 pinButtonReset,
            std::function<void()> cbSimpleReset,
            std::function<void()> cbLongReset
        );
        void init(
            u32 pinButtonReset,
            std::function<void()> cbSimpleReset,
            std::function<void()> cbLongReset,
            uint64 pressTime
        );
        void loop();
    private:
        u64 pressTime;
        u64 previous;
        u32 previousButtonState;
        u32 pinButtonReset;
        std::function<void()> cbSimpleReset;
        std::function<void()> cbLongReset;
        
        void callSimple();
        void callLong();
};

#endif