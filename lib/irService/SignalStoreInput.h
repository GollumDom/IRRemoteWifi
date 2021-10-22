#ifndef ir_SignalStoreInput_h
#define ir_SignalStoreInput_h

#include <IRremote.h>
#include <LJson.h>

class SignalStoreInput: public LDenormalized {

    public:
        String key;
        int id;
        virtual bool fromJson(LJsonNode* json);
};

#endif