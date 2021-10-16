#ifndef ljson_LJson_h
#define ljson_LJson_h

#include "vector"
#include "LJsonNode.h"
#include "LJsonScalar.h"
#include "LJsonArray.h"
#include "LJsonObject.h"
#include "LNormalized.h"
#include "LDenormalized.h"
#include "LParser.h"

#define VectorLJsonNode std::vector<LJsonNode*>
#define VectorLNormalize std::vector<LNormalized*>

String ljson_stringify(String value, boolean clear = false);
String ljson_stringify(int value, boolean clear = false);
String ljson_stringify(long value, boolean clear = false);
String ljson_stringify(float value, boolean clear = false);
String ljson_stringify(double value, boolean clear = false);
String ljson_stringify(bool value, boolean clear = false);
String ljson_stringify(LJsonNode* value, boolean clear = false);
String ljson_stringify(LNormalized* value, boolean clear = false);


String ljson_stringify(std::vector<String>* value, boolean clear = false);
String ljson_stringify(std::vector<int>* value, boolean clear = false);
String ljson_stringify(std::vector<long>* value, boolean clear = false);
String ljson_stringify(std::vector<float>* value, boolean clear = false);
String ljson_stringify(std::vector<double>* value, boolean clear = false);
String ljson_stringify(std::vector<bool>* value, boolean clear = false);
String ljson_stringify(VectorLJsonNode* value, boolean clear = false);
String ljson_stringify(VectorLNormalize* value, boolean clear = false);

LJsonNode* ljson_parse(String str);

LJsonNode* ljson_normalise(std::vector<LNormalized*>* value);

#endif