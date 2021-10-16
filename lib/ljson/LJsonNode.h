#ifndef ljson_LJsonNode_h
#define ljson_LJsonNode_h

#include "Arduino.h"

enum LJsonType {
    NULL_VALUE,
    STRING_VALUE,
    INT_VALUE,
    DEC_VALUE,
    BOOLEAN_VALUE,
    ARRAY_VALUE,
    OBJECT_VALUE,
};

class LJsonNode {
    public:
        virtual ~LJsonNode();
        virtual String toString();
        virtual LJsonType getType();
        virtual LJsonNode* clone();

        bool isObject();
        bool isArray();
        bool isScalar();
        bool isString();
        bool isNumber();
        bool isNumberInt();
        bool isNumberDec();
        bool isBool();

    protected:
        LJsonNode();
};

#endif