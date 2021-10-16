#ifndef helper_flashl_h
#define helper_flashl_h

#define led_flash(PIN, INTERVAL, FINISH) \
    digitalWrite(PIN, LOW); delay(INTERVAL);\
    digitalWrite(PIN, HIGH); delay(INTERVAL);\
    digitalWrite(PIN, LOW); delay(INTERVAL);\
    digitalWrite(PIN, HIGH); delay(INTERVAL);\
    digitalWrite(PIN, LOW); delay(INTERVAL);\
    digitalWrite(PIN, HIGH); delay(FINISH);

#endif