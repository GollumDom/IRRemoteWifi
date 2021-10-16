#include "LNormalized.h"
#include "LJsonScalar.h"

LNormalized::LNormalized() {}
LNormalized::~LNormalized() {}
LJsonNode* LNormalized::toJson() {
    return new LJsonScalar();
}
