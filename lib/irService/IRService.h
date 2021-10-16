#ifndef ir_IR_h
#define ir_IR_h

// #include <IRremote.h>
#include <vector>
#include <LJsonAsyncWebServer.h>
#include <helper.h>
#include "SignalData.h"

class IRService  {

    public:;
        virtual ~IRService();
        void init(
            LJsonAsyncWebServer* webServer,
            uint32 pinSender,
            uint32 pinReceiver,
            int32 pinLedSender = -1
        );
        void loop();

    private:
        LJsonAsyncWebServer* webServer = NULL;
        uint32 pinSender;
        uint32 pinReceiver;
        int32 pinLedSend;
        std::vector<SignalData*> signalDataHistory;
        std::vector<AsyncCallbackWebHandler*> webServerHandlers;

        void storeCode();
        void initWebServer();
        void clean();
};

#endif
