#ifndef json_LParser_h
#define json_LParser_h

#include "LJsonNode.h"
#include "LJsonScalar.h"
#include "LJsonObject.h"
#include "LJsonArray.h"

class LParser {

    public:

        LJsonNode* parse(char* str);
        LJsonNode* parse(String& str);
        bool hasError();

    private:
        bool _error = false;
        String* _buf;
        uint64 _cur = 0;
        uint64 _curTmp = 0;

        char _unescapeChar(char c);
        const char _char();
        const char _charTmp();
        
        LJsonNode* _parse();

        bool aggregateCheck(String &str, char check, bool onFoice = false);
        bool aggregateCheck(String &str, char start, char end, bool onFoice = false);

        bool _parseNumberDec(double &numberDec);
        bool _parseNumberInt(long &numberInt);
        bool _parseNumberBool(bool &numberBool);
        
        LJsonScalar* _parseString();
        LJsonArray* _parseArray();
        LJsonObject* _parseObject();

        void _parseWhite();
};

#endif