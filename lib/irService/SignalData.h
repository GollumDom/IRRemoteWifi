#ifndef ir_SignalData_h
#define ir_SignalData_h

#include <IRremote.h>
#include <LJson.h>

class SignalData: public LNormalized, public LDenormalized {

    public:
        int32 id;
        IRData receivedIRData;
        // extensions for sendRaw
        uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
        uint8_t rawCodeLength = 0; // The length of the code
        
        SignalData();

        virtual bool fromJson(LJsonNode* json);
        virtual LJsonNode* toJson();

};

#endif
