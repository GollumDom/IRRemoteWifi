#ifndef ljson_LJsonArray_h
#define ljson_LJsonArray_h

#include "LJsonNode.h"
#include "LJsonScalar.h"
#include "vector"

class LJsonObject;

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

        size_t length();

        LJsonNode* getChild(size_t index);
        LJsonObject* getChildObject(size_t index);
        LJsonArray*  getChildArray(size_t index);
        LJsonScalar* getChildScalar(size_t index);
        LJsonScalar* getChildNumberInt(size_t index);
        LJsonScalar* getChildNumberDec(size_t index);
        LJsonScalar* getChilString(size_t index);
        LJsonScalar* getChildBool(size_t index);

        virtual String toString();
        virtual LJsonType getType();
        virtual LJsonArray* clone();

    private:
        std::vector<LJsonNode*> children;

};

#endif