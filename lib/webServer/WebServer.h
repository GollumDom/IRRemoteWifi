#ifndef webServer_WebServer_h
#define webServer_WebServer_h

#include <helper.h>
#include <LJsonAsyncWebServer.h>

class WebServer {
    public:
        WebServer();
        ~WebServer();
        void init();
        void init(u32 port);
        void loop();
        LJsonAsyncWebServer* getWebServer();
    private:
        LJsonAsyncWebServer* webServer;
        void clean();
};

#endif
