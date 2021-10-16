#include "LJsonArray.h"
#include "LJsonScalar.h"

LJsonArray::LJsonArray() {}
LJsonArray::~LJsonArray() {
    for (LJsonNode* c: this->children) {
        delete c;
    }
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
