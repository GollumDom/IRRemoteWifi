#ifndef helper_serial_h
#define helper_serial_h

#define log_init(TIME) Serial.begin(TIME); delay(2000); Serial.println("");
#define log_l Serial.print
#define log_n Serial.println

#endif