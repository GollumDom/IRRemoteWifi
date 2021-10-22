#ifndef ljson_LJsonScalar_h
#define ljson_LJsonScalar_h

#include "LJsonNode.h"

class LJsonScalar : public LJsonNode {
    public:
        LJsonScalar();
        LJsonScalar(String value);
        LJsonScalar(int value    );
        LJsonScalar(long value   );
        LJsonScalar(float value  );
        LJsonScalar(double value );
        LJsonScalar(bool value   );
        virtual ~LJsonScalar();

        void setValue(); 
        void setValue(String value);
        void setValue(int value);
        void setValue(long value);
        void setValue(float value);
        void setValue(double value);
        void setValue(bool value);

        String getValueStr();
        int getValueInt();
        long getValueLong();
        bool getValueBool();
        float getValueFloat();
        double getValueDouble();

        virtual String toString();
        virtual LJsonType getType();
        virtual LJsonScalar* clone();

    private:
        LJsonType type;
        String valueStr;
        long valueInt;
        double valueDec;
        boolean valueBool;
};

#endif