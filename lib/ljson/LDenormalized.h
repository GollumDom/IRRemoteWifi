#ifndef json_LDenormalized_h
#define json_LDenormalized_h

#include "LJsonNode.h"
#include "LJsonScalar.h"
#include "vector"

class LDenormalized {
    public:
        LDenormalized();
        virtual ~LDenormalized();
        virtual bool fromJson(LJsonNode* json);
};

#endif