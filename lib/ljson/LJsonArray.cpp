#include "LJsonArray.h"
#include "LJsonObject.h"

LJsonArray::LJsonArray() {}
LJsonArray::~LJsonArray() {
    for (LJsonNode* c: this->children) {
        delete c;
    }
    this->children.clear();
}

LJsonArray* LJsonArray::addChild(String child) { return this->addChild(new LJsonScalar(child)); }
LJsonArray* LJsonArray::addChild(int child   ) { return this->addChild(new LJsonScalar(child)); }
LJsonArray* LJsonArray::addChild(long child  ) { return this->addChild(new LJsonScalar(child)); }
LJsonArray* LJsonArray::addChild(float child ) { return this->addChild(new LJsonScalar(child)); }
LJsonArray* LJsonArray::addChild(double child) { return this->addChild(new LJsonScalar(child)); }
LJsonArray* LJsonArray::addChild(bool child  ) { return this->addChild(new LJsonScalar(child)); }

LJsonArray* LJsonArray::addChild(LJsonNode* child) {
    this->children.push_back(child);
    return this;
}

size_t LJsonArray::length() {
    return this->children.size();
}

LJsonNode* LJsonArray::getChild(size_t index) {
    return index < this->children.size() ? this->children[index] : NULL;
}
LJsonObject* LJsonArray::getChildObject   (size_t index) { LJsonNode* c = this->getChild(index); return c && c->isObject()    ? (LJsonObject*)c : NULL; }
LJsonArray*  LJsonArray::getChildArray    (size_t index) { LJsonNode* c = this->getChild(index); return c && c->isArray()     ? (LJsonArray* )c : NULL; }
LJsonScalar* LJsonArray::getChildScalar   (size_t index) { LJsonNode* c = this->getChild(index); return c && c->isScalar()    ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonArray::getChildNumberInt(size_t index) { LJsonNode* c = this->getChild(index); return c && c->isNumberInt() ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonArray::getChildNumberDec(size_t index) { LJsonNode* c = this->getChild(index); return c && c->isNumberDec() ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonArray::getChilString    (size_t index) { LJsonNode* c = this->getChild(index); return c && c->isString()    ? (LJsonScalar*)c : NULL; }
LJsonScalar* LJsonArray::getChildBool     (size_t index) { LJsonNode* c = this->getChild(index); return c && c->isBool()      ? (LJsonScalar*)c : NULL; }


String LJsonArray::toString() {
    String content;
    long i = 0;
    for (LJsonNode* c: this->children) {
        content += (i == 0 ? "" : ",") + c->toString();
        i++;
    }
    return F("[") + content + F("]");
}

LJsonType LJsonArray::getType() {
    return ARRAY_VALUE;
}

LJsonArray* LJsonArray::clone() {
    LJsonArray* clone = new LJsonArray();
    for (LJsonNode* c: this->children) {
        clone->addChild(c->clone());
    }
    return clone;
}
