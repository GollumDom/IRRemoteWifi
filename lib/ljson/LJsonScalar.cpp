#include "LJsonScalar.h"

LJsonScalar::LJsonScalar() {
    this->type = NULL_VALUE;
    this->valueStr = "";
    this->valueInt = 0;
    this->valueDec = 0;
    this->valueBool = false;
}
LJsonScalar::LJsonScalar(String value): LJsonScalar() { this->setValue(value); }
LJsonScalar::LJsonScalar(int value   ): LJsonScalar() { this->setValue(value); }
LJsonScalar::LJsonScalar(long value  ): LJsonScalar() { this->setValue(value); }
LJsonScalar::LJsonScalar(float value ): LJsonScalar() { this->setValue(value); }
LJsonScalar::LJsonScalar(double value): LJsonScalar() { this->setValue(value); }
LJsonScalar::LJsonScalar(bool value  ): LJsonScalar() { this->setValue(value); }
LJsonScalar::~LJsonScalar() {}

void LJsonScalar::setValue()              { this->type = NULL_VALUE;  }
void LJsonScalar::setValue(String value)  { this->type = STRING_VALUE; this->valueStr = value; }
void LJsonScalar::setValue(int value)     { this->type = INT_VALUE; this->valueInt = value; }
void LJsonScalar::setValue(long value)    { this->type = INT_VALUE; this->valueInt = value; }
void LJsonScalar::setValue(float value)   { this->type = DEC_VALUE; this->valueDec = value; }
void LJsonScalar::setValue(double value)  { this->type = DEC_VALUE; this->valueDec = value; }
void LJsonScalar::setValue(bool value)    { this->type = BOOLEAN_VALUE; this->valueBool = value; }


String LJsonScalar::getValueStr() {
    switch (this->type) {
        case STRING_VALUE: return String(this->valueStr);
        case NULL_VALUE: return "null";
        case BOOLEAN_VALUE: return this->valueBool ? "true" : "false";
        case DEC_VALUE: return String(this->valueDec);
        default: return String(this->valueInt);
    }
}

String LJsonScalar::toString() {
    String result;
    switch (this->type) {
        case STRING_VALUE:
            result = String(this->valueStr);
            result.replace("\\", "\\\\");
            result.replace("\n", "\\n");
            result.replace("\r", "\\r");
            result.replace("\t", "\\t");
            result.replace("\"", "\\\"");
            return F("\"")+result+F("\"");

        case NULL_VALUE: return "null";
        case BOOLEAN_VALUE: return this->valueBool ? "true" : "false";
        case DEC_VALUE: return String(this->valueDec);
        default: return String(this->valueInt);
    }
}

LJsonType LJsonScalar::getType() {
    return this->type;
}

LJsonScalar* LJsonScalar::clone() {
    LJsonScalar* clone = new LJsonScalar();
    clone->type = this->type;
    clone->valueStr = this->valueStr;
    clone->valueDec = this->valueDec;
    clone->valueInt = this->valueInt;
    clone->valueBool = this->valueBool;
    return clone;
}