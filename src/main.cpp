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

Reset reset;
WifiService wifiService;
FileFS fileFS;
WebServer webServer;
IRService irService;
uint64 previous = 0;

void setup() {

    log_init(115200);

    log_n(F("START " __FILE__ " from " __DATE__ ));

    fileFS.init([]() {
        // On error initialize FS
        pinMode(PIN_LED_WIFI, OUTPUT);
        while(1) {
            led_flash(PIN_LED_WIFI, 100, 1000);
        }
    });
    webServer.init();
    wifiService.init(PROJECT_NAME, &fileFS, webServer.getWebServer(), PIN_LED_WIFI);

    irService.init(
        webServer.getWebServer(),
        &fileFS,
        PIN_SENDER,
        PIN_RECEIVER,
        PIN_LED_SEND
    );

    reset.init(
        PIN_BUTTON_RESET,
        
        // Simple reset
        []() {
            wifiService.startRescueMode(); 
        },

        // Long reset
        []() {
            wifiService.clearConfig();
            irService.clearStoredSignals();
            ESP.restart();
        }
    );

}

void loop() {
    reset.loop();
    fileFS.loop();
    webServer.loop();
    wifiService.loop();
    irService.loop();

    if (ESP.getFreeHeap() < 5000) {
        ESP.restart();
    }
    uint64 now = millis();
    if (now - previous > 2000) {
        previous = now;
        log_l("MEMORY: "); log_l(ESP.getFreeHeap())
        ;log_l(" MFB: "); log_l(ESP.getMaxFreeBlockSize());
        ;log_l(" FRAG: "); log_n(ESP.getHeapFragmentation());
    }

}