#include "SignalStoreInput.h"

bool SignalStoreInput::fromJson(LJsonNode* json) {

    if (!json->isObject()) return false;
    
    LJsonObject* obj = (LJsonObject*)json;
    if (LJsonScalar* c = obj->getChildNumberInt("id")) this->id  = c->getValueInt(); else return false;
    if (LJsonScalar* c = obj->getChilString("key")   ) this->key = c->getValueStr(); else return false;
    
    return true;
}
