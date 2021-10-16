#include "LJsonNode.h"


LJsonNode::LJsonNode() {}
LJsonNode::~LJsonNode() {}

String LJsonNode::toString() {
    return "null";
}

LJsonType LJsonNode::getType() {
    return NULL_VALUE;
}

bool LJsonNode::isObject() { return this->getType() == OBJECT_VALUE; }
bool LJsonNode::isArray() { return this->getType() == ARRAY_VALUE; }
bool LJsonNode::isNumberInt() { return this->getType() == INT_VALUE; }
bool LJsonNode::isNumberDec() { return this->getType() == DEC_VALUE; }
bool LJsonNode::isBool() { return this->getType() == BOOLEAN_VALUE; }
bool LJsonNode::isString() { return this->getType() == STRING_VALUE; }
bool LJsonNode::isScalar() {
    return this->isNumber() || this->isString() || this->isBool(); 
}
bool LJsonNode::isNumber() { 
    return this->isNumberDec() || this->isNumberInt();
}

LJsonNode* LJsonNode::clone() {
    return NULL;
}
