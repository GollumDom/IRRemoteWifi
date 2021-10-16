#ifndef ljson_LJsonArray_h
#define ljson_LJsonArray_h

#include "LJsonNode.h"
#include "vector"

class LJsonArray : public LJsonNode {
    public:
        LJsonArray();
        virtual ~LJsonArray();
        
        LJsonArray* addChild(String child);
        LJsonArray* addChild(int child);
        LJsonArray* addChild(long child);
        LJsonArray* addChild(float child);
        LJsonArray* addChild(double child);
        LJsonArray* addChild(bool child);
        LJsonArray* addChild(LJsonNode* child);
        virtual String toString();
        virtual LJsonType getType();
        virtual LJsonArray* clone();

    private:
        std::vector<LJsonNode*> children;

};

#endif