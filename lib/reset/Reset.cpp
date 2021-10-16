#include "Reset.h"

Reset::Reset() {}

Reset::~Reset() {}

void Reset::init(
    u32 pinButtonReset,
    std::function<void()> cbSimpleReset,
    std::function<void()> cbLongReset
) {
    this->init(
        pinButtonReset,
        cbSimpleReset,
        cbLongReset,
        5000
    );
}

void Reset::init(
    u32 pinButtonReset,
    std::function<void()> cbSimpleReset,
    std::function<void()> cbLongReset,
    uint64 pressTime
) {
    this->pinButtonReset = pinButtonReset;
    this->cbSimpleReset = cbSimpleReset;
    this->cbLongReset = cbLongReset;
    this->pressTime = pressTime;

    pinMode(this->pinButtonReset, INPUT_PULLUP);
    this->previousButtonState = HIGH;
    this->previous = millis();
}

void Reset::loop() {

    u64 now = millis();
    u32 buttonState = digitalRead(this->pinButtonReset);
    
    if (this->previousButtonState == LOW) {

        if (buttonState == LOW) {

            if (now - previous > this->pressTime) {
                this->previous = now;
                this->previousButtonState = buttonState;
                this->callLong();
            }
        } else {
            this->previous = now;
            this->previousButtonState = buttonState;
            this->callSimple();
        }

    } else {
        if (buttonState == LOW) {
            this->previous = now;
        }
        this->previousButtonState = buttonState;
    }
}

void Reset::callSimple() {
    log_n("simple Reset called");
    this->cbSimpleReset();
}

void Reset::callLong() {
    log_n("Long Reset called");
    this->cbLongReset();
}