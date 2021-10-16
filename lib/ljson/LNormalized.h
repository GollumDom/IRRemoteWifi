#ifndef json_LNormalized_h
#define json_LNormalized_h

#include "LJsonNode.h"
#include "LJsonScalar.h"
#include "vector"

class LNormalized {
    public:
        LNormalized();
        virtual ~LNormalized();
        virtual LJsonNode* toJson();
};

#endif