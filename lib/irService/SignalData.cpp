#include "SignalData.h"

int signalCounter = 0;

SignalData::SignalData() {
    this->id = ++signalCounter;
}

bool SignalData::fromJson(LJsonNode* json) {

    if (!json->isObject()) return false;
    
    LJsonObject* obj = (LJsonObject*)json;
    if (LJsonScalar* c = obj->getChildNumberInt("id")) this->id  = c->getValueInt(); else return false;
    
    if (LJsonObject* o = obj->getChildObject("received_ir_data")) {
        if (LJsonScalar* c = o->getChildNumberInt("protocol")      ) this->receivedIRData.protocol       = (decode_type_t)c->getValueInt(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("address")       ) this->receivedIRData.address        = c->getValueLong(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("command")       ) this->receivedIRData.command        = c->getValueLong(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("extra")         ) this->receivedIRData.extra          = c->getValueLong(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("numberOfBits")  ) this->receivedIRData.numberOfBits   = c->getValueLong(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("flags")         ) this->receivedIRData.flags          = c->getValueLong(); else return false;
        if (LJsonScalar* c = o->getChildNumberInt("decodedRawData")) this->receivedIRData.decodedRawData = c->getValueLong(); else return false;
    } else {
        return false;
    }
    
    if (LJsonArray* ar = obj->getChildArray("raw_code")) {
        this->rawCodeLength = ar->length();
        for (size_t i = 0; i < this->rawCodeLength; i++) {
            if (LJsonScalar* c = ar->getChildNumberInt(i)) {
                this->rawCode[i] = c->getValueInt();
            } else {
                return false;
            }
        }
     } else {
         return false;
     }

    return true;
}

LJsonNode* SignalData::toJson() {

    LJsonArray* rawCode = new LJsonArray();
    for (int i = 0; i < this->rawCodeLength; i++) {
        rawCode->addChild(this->rawCode[i]);
    }

    LJsonObject* obj = (new LJsonObject())
        ->addChild("id", this->id)
        ->addChild("received_ir_data", (new LJsonObject)
            ->addChild("protocol"      , this->receivedIRData.protocol)
            ->addChild("address"       , this->receivedIRData.address)
            ->addChild("command"       , this->receivedIRData.command)
            ->addChild("extra"         , this->receivedIRData.extra)
            ->addChild("numberOfBits"  , this->receivedIRData.numberOfBits)
            ->addChild("flags"         , this->receivedIRData.flags)
            ->addChild("decodedRawData", (long)this->receivedIRData.decodedRawData)
        )
        ->addChild("raw_code", rawCode)
    ;
    return obj;
}
