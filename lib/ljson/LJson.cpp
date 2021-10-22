#include "LJson.h"
#include "LJsonScalar.h"

String ljson_stringify(String  value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }
String ljson_stringify(int     value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }
String ljson_stringify(long    value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }
String ljson_stringify(float   value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }
String ljson_stringify(double  value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }
String ljson_stringify(bool    value, boolean clear) { String rtn = ljson_stringify(new LJsonScalar(value), true); if (clear) delete &value; return rtn; }

String ljson_stringify(LJsonNode* value, boolean clear) {
    String rtn = value->toString();
    if (clear) delete value;
    return rtn;
}

String ljson_stringify(LNormalized* value, boolean clear) {
    LJsonNode* json = value->toJson();
    if (clear) delete value;
    return ljson_stringify(json, true);
    
}

String ljson_stringify(std::vector<String>*  value, boolean clear) { LJsonArray array; for (String  c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }
String ljson_stringify(std::vector<int>*     value, boolean clear) { LJsonArray array; for (int     c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }
String ljson_stringify(std::vector<long>*    value, boolean clear) { LJsonArray array; for (long    c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }
String ljson_stringify(std::vector<float>*   value, boolean clear) { LJsonArray array; for (float   c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }
String ljson_stringify(std::vector<double>*  value, boolean clear) { LJsonArray array; for (double  c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }
String ljson_stringify(std::vector<bool>*    value, boolean clear) { LJsonArray array; for (bool    c: *value) { array.addChild(new LJsonScalar(c)); } if (clear) delete value; return array.toString(); }


String ljson_stringify(VectorLJsonNode* value, boolean clear) {
    LJsonArray array;
    for (LJsonNode* i: *value) {
        array.addChild(i->clone());
        if (clear) delete i;
    }
    if (clear) delete value;
    return array.toString();
}

String ljson_stringify(VectorLNormalize* value, boolean clear) {
    LJsonArray array;
    for (LNormalized* i: *value) {
        array.addChild(i->toJson());
        if (clear) delete i;
    }
    if (clear) delete value;
    return array.toString();
}

LJsonNode* ljson_parse(String str) {
    LParser parser;
    return parser.parse(str);
}

bool ljson_parse(String str, LDenormalized* obj) {
    LJsonNode* json = ljson_parse(str);
    if (!json) return false;
    bool result = obj->fromJson(json);
    delete json;
    return result;
}

LJsonNode* ljson_normalize(VectorLNormalize* value) {
    LJsonArray* array = new LJsonArray();
    for (LNormalized* i: *value) {
        LJsonNode* c = i->toJson();
        array->addChild(c);
    }
    return array;
}