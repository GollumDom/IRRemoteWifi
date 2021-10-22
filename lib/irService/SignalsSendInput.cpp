#include "SignalsSendInput.h"

SignalsSendInput::~SignalsSendInput() {
    this->clean();
}

void SignalsSendInput::clean() {
    for (SignalSend* signal: this->signals) {
        delete signal;
    }
    this->signals.clear();
}

bool SignalsSendInput::fromJson(LJsonNode* json) {

    if (!json->isArray()) return false;

    LJsonArray* ar = (LJsonArray*) json;
    for (size_t i = 0; i < ar->length(); i++) {
        LJsonNode* c = ar->getChild(i);
        SignalSend* signal = new SignalSend;
        if (signal->fromJson(c)) {
            this->signals.push_back(signal);
        } else {
            delete signal;
            this->clean();
            return false;
        }
    }

    return true;
}

LJsonNode* SignalsSendInput::toJson(){
    // return ljson_normalize((VectorLNormalize*)&this->signals); // Unknow bug with call normalize FfromJSON is called where as toJSon...
    LJsonArray* array = new LJsonArray();
    for (LNormalized* i: this->signals) {
        LJsonNode* c = i->toJson();
        array->addChild(c);
    }
    return array;
}