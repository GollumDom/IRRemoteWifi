#include "LJsonObject.h"
#include "LJsonScalar.h"

LJsonObject::LJsonObject() {}

LJsonObject::~LJsonObject() {
    for (std::pair<String, LJsonNode*> entry: this->children) {
        delete entry.second;
    }
}

LJsonObject* LJsonObject::addChild(String key, String child) { return this->addChild(key, new LJsonScalar(child)); }
LJsonObject* LJsonObject::addChild(String key, int child   ) { return this->addChild(key, new LJsonScalar(child)); }
LJsonObject* LJsonObject::addChild(String key, long child  ) { return this->addChild(key, new LJsonScalar(child)); }
LJsonObject* LJsonObject::addChild(String key, float child ) { return this->addChild(key, new LJsonScalar(child)); }
LJsonObject* LJsonObject::addChild(String key, double child) { return this->addChild(key, new LJsonScalar(child)); }
LJsonObject* LJsonObject::addChild(String key, bool child  ) { return this->addChild(key, new LJsonScalar(child)); }

LJsonObject* LJsonObject::addChild(String key, LJsonNode* child) {
    this->children.insert(std::make_pair(key, child));
    return this;
}

LJsonNode* LJsonObject::getChild(String key) {
    return this->children.count(key) ? this->children.find(key)->second : NULL;
}

LJsonScalar* LJsonObject::getChildScalar      (String key) { LJsonNode* c = this->getChild(key); return c && c->isScalar()    ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonObject::getChildNumberInt   (String key) { LJsonNode* c = this->getChild(key); return c && c->isNumberInt() ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonObject::getChildNumberDec   (String key) { LJsonNode* c = this->getChild(key); return c && c->isNumberDec() ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonObject::getChildNumberString(String key) { LJsonNode* c = this->getChild(key); return c && c->isString()    ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonObject::getChildNumberBool  (String key) { LJsonNode* c = this->getChild(key); return c && c->isBool()      ? (LJsonScalar*)c : NULL; }
LJsonArray*  LJsonObject::getChildNumberArray (String key) { LJsonNode* c = this->getChild(key); return c && c->isArray()     ? (LJsonArray*)c  : NULL; }
LJsonObject* LJsonObject::getChildNumberObject(String key) { LJsonNode* c = this->getChild(key); return c && c->isObject()    ? (LJsonObject*)c : NULL; }


String LJsonObject::toString() {
    String content;
    long i = 0;
    for (std::pair<String, LJsonNode*> entry: this->children) {
        content += (i == 0 ? "" : ",") + LJsonScalar(entry.first).toString()+":"+entry.second->toString();
        i++;
    }
    return F("{") + content + F("}");
}

LJsonType LJsonObject::getType() {
    return OBJECT_VALUE;
}

LJsonObject* LJsonObject::clone() {
    LJsonObject* clone = new LJsonObject();
    for (std::pair<String, LJsonNode*> entry: this->children) {
        clone->addChild(entry.first, entry.second->clone());
    }
    return clone;
}