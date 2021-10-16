#include "LParser.h"


LJsonNode* LParser::parse(char* str) {
    String strObj(str);
    return this->parse(strObj);
}

LJsonNode* LParser::parse(String& str) {
    this->_error = false;
    this->_cur = 0;
    this->_buf = &str;
    LJsonNode* json = this->_parse();
    if (this->_cur != this->_buf->length()) {
        this->_error = true;
        if (json) {
            delete json;
        }
        json = NULL;
    }
    return json;
}

bool LParser::hasError() {
    return this->_error;
}

char LParser::_unescapeChar(char c) {
    const char *p = "//\"\"\\\\b\bf\fn\nr\rt\t";
    for (;;) {
        if (p[0] == '\0') {
            return 0;
        }
        if (p[0] == c) {
            return p[1];
        }
        p += 2;
    }
}

const char LParser::_char() {
    return (*this->_buf)[this->_cur];
}

const char LParser::_charTmp() {
    return (*this->_buf)[this->_curTmp];
}

LJsonNode* LParser::_parse() {

    LJsonNode* rtn = NULL;
    
    this->_parseWhite();

    if (this->_cur == this->_buf->length()) {
        this->_error = true;
        return NULL;
    }

    double numberDec;
    long numberInt;
    bool numberBool;
    
    if (this->_parseNumberDec(numberDec)) {
        rtn = new LJsonScalar(numberDec);
    } else
    if (this->_parseNumberInt(numberInt)) {
        rtn = new LJsonScalar(numberInt);
    }
    if (this->_parseNumberBool(numberBool)) {
        rtn = new LJsonScalar(numberBool);
    } else {
        const char c = this->_char();

        if (c == '"') {
            rtn = this->_parseString();
        } else
        if (c == '[') {
            rtn = this->_parseArray();
        } else
        if (c == '{') {
            rtn = this->_parseObject();
        }

    }

    this->_parseWhite();
    
    if (!rtn) {
        this->_error = true;
    }


    if (this->_error) {
        if (rtn) {
            delete rtn;
        }
        return NULL;
    }

    return rtn;
}


bool LParser::aggregateCheck(String &str, char check, bool onFoice) {
    return aggregateCheck(str, check, check, onFoice);
}

bool LParser::aggregateCheck(String &str, char start, char end, bool onFoice) {
    if (this->_curTmp == this->_buf->length()) {
        return false;
    }
    char c = this->_charTmp();

    bool isEnter = false;
    while (c >= start && c <= end) {
        str += c;
        this->_curTmp++;
        isEnter = true;
        if (this->_cur == this->_buf->length()) {
            return true;
        }
        if (onFoice) {
            break;
        }
        c = this->_charTmp();
    }
    return isEnter;
}

bool LParser::_parseNumberDec(double &numberDec) {

    if (this->_cur == this->_buf->length()) {
        return false;
    }
    
    this->_curTmp = this->_cur;
    
    String agreagate = "";
    
    if (!aggregateCheck(agreagate, '0', '9')) {
        return false;
    }
    
    if (aggregateCheck(agreagate, '.', true)) {

        if (!aggregateCheck(agreagate, '0', '9')) {
            return false;
        }

        if (aggregateCheck(agreagate, 'e', true)) {
            if (!aggregateCheck(agreagate, '0', '9')) {
                return false;
            }
        }

    } else if (aggregateCheck(agreagate, 'e', true)) {
        if (!aggregateCheck(agreagate, '0', '9')) {
            return false;
        }
    } else {
        return false;
    }

    char * endPtr;
    numberDec = std::strtod(agreagate.c_str(), &endPtr);
    this->_cur = this->_curTmp;
    return true;
}

bool LParser::_parseNumberInt(long &numberInt) {

    if (this->_cur == this->_buf->length()) {
        return false;
    }
    
    this->_curTmp = this->_cur;
    
    String agreagate = "";
    
    if (!aggregateCheck(agreagate, '0', '9')) {
        return false;
    }
    
    char * endPtr;
    numberInt = std::strtol(agreagate.c_str(), &endPtr, 10);
    this->_cur = this->_curTmp;
    return true;
}

bool LParser::_parseNumberBool(bool &numberBool) {

    if (this->_cur == this->_buf->length()) {
        return false;
    }

    uint64 cur4 = this->_cur + 4;
    if (cur4 <= this->_buf->length()) {
        String sub = this->_buf->substring(this->_cur, cur4);
        sub.toLowerCase();
        if (sub == "true") {
            numberBool = true;
            this->_cur = cur4;
            return true;
        }
    }


    uint64 cur5 = this->_cur + 5;
    if (cur5 <= this->_buf->length()) {
        String sub = this->_buf->substring(this->_cur, cur5);
        sub.toLowerCase();
        if (sub == "false") {
            numberBool = false;
            this->_cur = cur5;
            return true;
        }
    }

    return false;

}

LJsonScalar* LParser::_parseString() {
    char c = this->_char();
    if (c != '"') {
        return NULL;
    }
        
    String value = "";

    while (true) {
        this->_cur++;
        if (this->_cur == this->_buf->length()) {
            return NULL;
        }
        c = this->_char();

        if (c == '"') {
            break;
        }

        if (c == '\0') {
            return NULL;
        }

        if (c == '\\') {
            this->_cur++;
            if (this->_cur == this->_buf->length()) {
                return NULL;
            }
            c = this->_char();
            if (c == '\0') {
                return NULL;
            }

            // TODO parse utf8 (show https://github.com/bblanchon/ArduinoJson/blob/6.x/src/ArduinoJson/Json/JsonDeserializer.hpp line 379)

            c = this->_unescapeChar(c);
            if (c == '\0') {
                return NULL;
            }
        }

        value += c;
    }

    this->_cur++;
    return new LJsonScalar(value);
}

LJsonArray* LParser::_parseArray() {
    if (this->_char() != '[') {
        return NULL;
    }
    this->_cur++;
    this->_parseWhite();
    if (this->_cur == this->_buf->length()) {
        return NULL;
    }
    

    LJsonArray* json = new LJsonArray();
    

    if (this->_char() == ']') {
        this->_cur++;
    } else {
        while (true) {
            
            LJsonNode* child = this->_parse();
            if (this->_error) {
                if (child) {
                    delete child;
                }
                delete json;
                return NULL;
            }
            json->addChild(child);


            if (this->_cur == this->_buf->length()) {
                delete json;
                return NULL;
            }
            
            const char c = this->_char();
            if (c == ']') {
                this->_cur++;
                break;
            }
            
            if (c != ',') {
                delete json;
                return NULL;
            }
            this->_cur++;
        }
    }

    return json;
}

LJsonObject* LParser::_parseObject() {
    if (this->_char() != '{') {
        return NULL;
    }
    this->_cur++;
    this->_parseWhite();
    if (this->_cur == this->_buf->length()) {
        return NULL;
    }

    LJsonObject* json = new LJsonObject();

    if (this->_char() == '}') {
        this->_cur++;
    } else {

        while (true) {
            
            
            this->_parseWhite();
            
            if (this->_cur == this->_buf->length()) {
                delete json;
                return NULL;
            }

            LJsonScalar* key = this->_parseString();

            if (!key || key->getType() != STRING_VALUE) {
                if (key) {
                    delete key;
                }
                delete json;
                return NULL;
            }

            
            this->_parseWhite();
            
            if (this->_cur == this->_buf->length()) {
                delete key;
                delete json;
                return NULL;
            }

            if (this->_char() != ':') {
                delete key;
                delete json;
                return NULL;
            }
            this->_cur++;

            if (this->_cur == this->_buf->length()) {
                delete key;
                delete json;
                return NULL;
            }

            LJsonNode* child = this->_parse();
            if (this->_error) {
                if (child) {
                    delete child;
                }
                delete key;
                delete json;
                return NULL;
            }
            
            json->addChild(key->getValueStr(), child);
            delete key;

            if (this->_cur == this->_buf->length()) {
                delete json;
                return NULL;
            }


            const char c = this->_char();
            if (c == '}') {
                this->_cur++;
                break;
            }
            
            if (c != ',') {
                delete json;
                return NULL;
            }

            this->_cur++;
            if (this->_cur == this->_buf->length()) {
                delete json;
                return NULL;
            }

        }
    }

    return json;
}

void LParser::_parseWhite() {
    if (this->_cur == this->_buf->length()) {
        return;
    }
    char c = this->_char();
    while (
        c == ' ' ||
        c == '\r' ||
        c == '\t' ||
        c == '\n' 
    ) {
        this->_cur++;
        if (this->_cur == this->_buf->length()) {
            return;
        }
        c = this->_char();
    }
}