#include "SignalSend.h"

SignalSend::SignalSend() {
    this->repeat = 0;
    this->wait = 0;
    this->prevouis = 0;
    this->state = IDLE;
}


bool SignalSend::fromJson(LJsonNode* json) {

    if (!json->isObject()) return false;

    LJsonObject* obj = (LJsonObject*)json;
    if (LJsonScalar* c = obj->getChilString("value")) this->value = c->getValueStr(); else return false;
    if (obj->hasChild("repeat")) { if (LJsonScalar* c = obj->getChildNumberInt("repeat")) this->repeat = c->getValueInt(); else return false; }
    if (obj->hasChild("wait"  )) { if (LJsonScalar* c = obj->getChildNumberInt("wait"  )) this->wait   = c->getValueInt(); else return false; }

    return true;
}

LJsonNode* SignalSend::toJson() {
    return (new LJsonObject())
        ->addChild("value", this->value)
        ->addChild("repeat", (int)this->repeat)
        ->addChild("wait", (int)this->wait)
    ;
}

SignalSend* SignalSend::clone() {
    SignalSend* clone = new SignalSend();
    clone->value = this->value;
    clone->wait = this->wait;
    clone->repeat = this->repeat;
    clone->state = this->state;
    clone->prevouis = this->prevouis;
    return clone;
}