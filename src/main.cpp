#include <Arduino.h>
#include <helper.h>
#include <Reset.h>
#include <WifiService.h>
#include <FileFS.h>
#include <WebServer.h>
#include <IRService.h>

#define PROJECT_NAME "REMOTE_CONTROLE"
#define PIN_RECEIVER PIN_D1
#define PIN_SENDER PIN_D6  
#define PIN_LED_SEND PIN_D3
#define PIN_LED_WIFI PIN_D7
#define PIN_BUTTON_RESET PIN_D5
// #define DELAY_BETWEEN_REPEAT 50

Reset reset;
WifiService wifiService;
FileFS fileFS;
WebServer webServer;
IRService irService;

// void sendCode(storedIRDataStruct *aIRDataToSend) {
//     if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */) {
//         // Assume 38 KHz
//         IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

//         Serial.print(F("Sent raw "));
//         Serial.print(aIRDataToSend->rawCodeLength);
//         Serial.println(F(" marks or spaces"));
//     } else {

//         /*
//          * Use the write function, which does the switch for different protocols
//          */
//         IrSender.write(&aIRDataToSend->receivedIRData, NO_REPEATS);

//         Serial.print(F("Sent: "));
//         printIRResultShort(&Serial, &aIRDataToSend->receivedIRData);
//     }
// }

void setup() {

    log_init(115200);

    log_n(F("START " __FILE__ " from " __DATE__ ));
    log_n("Initialize PIN");

//    pinMode(PIN_BUTTON_SEND, INPUT_PULLUP); 

    fileFS.init([]() {
        // On error initialize FS
        pinMode(PIN_LED_WIFI, OUTPUT);
        while(1) {
            led_flash(PIN_LED_WIFI, 100, 1000);
        }
    });
    webServer.init();
    wifiService.init(PROJECT_NAME, &fileFS, webServer.getWebServer(), PIN_LED_WIFI);

    reset.init(
        PIN_BUTTON_RESET,
        
        // Simple reset
        []() {
            wifiService.startRescueMode(); 
        },

        // Long reset
        []() {
            wifiService.clearConfig();
            ESP.restart();
        }
    );

    irService.init(
        webServer.getWebServer(),
        PIN_SENDER,
        PIN_RECEIVER,
        PIN_LED_SEND
    );

}

void loop() {

    reset.loop();
    fileFS.loop();
    webServer.loop();
    wifiService.loop();
    irService.loop();
    
    // // If button pressed, send the code.
    // int buttonSendState = digitalRead(PIN_BUTTON_SEND); // Button pin is active LOW

    // // Check for button just released in order to activate receiving
    // if (lastButtonState == LOW && buttonSendState == HIGH) {
    //     // Re-enable receiver
    //     Serial.println(F("Button released"));
    //     IrReceiver.start();
    // }

    // // Check for static button state
    // if (buttonSendState == LOW) {
    //     IrReceiver.stop();
        
    //     // Button pressed send stored data or repeat
    //     Serial.println(F("Button pressed, now sending"));
    //     digitalWrite(PIN_LED_PRESSED, HIGH);
    //     if (lastButtonState == buttonSendState) {
    //         sStoredIRData.receivedIRData.flags = IRDATA_FLAGS_IS_REPEAT;
    //     }
    //     sendCode(&sStoredIRData);
    //     digitalWrite(PIN_LED_PRESSED, LOW);
    //     delay(DELAY_BETWEEN_REPEAT); // Wait a bit between retransmissions

    
    // lastButtonState = buttonSendState;
}