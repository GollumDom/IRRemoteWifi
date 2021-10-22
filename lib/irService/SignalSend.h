#ifndef SignalSend_h
#define SignalSend_h

#include <Arduino.h>
#include <LJson.h>

enum SignalSendState {
    IDLE,
    SEND,
    WAIT
};

class SignalSend: public LDenormalized, public LNormalized {

    public:
        SignalSend();
        String value;
        uint32 repeat;
        uint32 wait;
        uint64 prevouis; 
        SignalSendState state;
        virtual bool fromJson(LJsonNode* json);
        virtual LJsonNode* toJson();
        virtual SignalSend* clone();
};


#endif