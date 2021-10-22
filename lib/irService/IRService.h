#ifndef ir_IR_h
#define ir_IR_h

#include <vector>
#include <LJsonAsyncWebServer.h>
#include <helper.h>
#include <FileFS.h>
#include "SignalData.h"
#include "SignalStoreInput.h"
#include "SignalsSendInput.h"

#define STORE_PATH "/remote"
#define DELAY_BETWEEN_REPEAT 50

class IRService  {

    public:;
        IRService();
        virtual ~IRService();
        void init(
            LJsonAsyncWebServer* webServer,
            FileFS* fileFS,
            uint32 pinSender,
            uint32 pinReceiver,
            int32 pinLedSender = -1
        );
        void loop();
        void clearStoredSignals();

    private:
        LJsonAsyncWebServer* webServer;
        FileFS* fileFS;
        uint32 pinSender;
        uint32 pinReceiver;
        int32 pinLedSend;
        std::vector<SignalData*> signalDataHistory;
        std::vector<AsyncCallbackWebHandler*> webServerHandlers;
        std::vector<SignalSend*> signalsSend;

        void storeCode();
        void initWebServer();
        void clean();
        void sendSignal(String key, bool repeat = false);
        SignalData* findSignalDataInHistoryById(int id);
        void storeSignal(String key, SignalData* signal);
        SignalData* findSignalDataByKey(String key);
        std::vector<String> listSignalStored();
};

#endif
