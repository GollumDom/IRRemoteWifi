#ifndef SignalsSendInput_h
#define SignalsSendInput_h

#include "LJson.h"
#include "SignalSend.h"

class SignalsSendInput: public LDenormalized, public LNormalized {
    public:
        std::vector<SignalSend*> signals;

        virtual ~SignalsSendInput();
        virtual bool fromJson(LJsonNode* json);
        virtual LJsonNode* toJson();

    private:
        void clean();
};

#endif