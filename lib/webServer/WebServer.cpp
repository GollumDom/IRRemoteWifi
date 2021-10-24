#include "WebServer.h"
#include "LittleFS.h"

WebServer::WebServer() {
    this->webServer = NULL;
}

WebServer::~WebServer() {
    this->clean();
}

void WebServer::init() {
    this->init(80);
}

void WebServer::init(u32 port) {
    this->clean();
    this->webServer = new LJsonAsyncWebServer(port);

    this->webServer->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "application/json", "{\"error\":\"Page not found\"}");
    });

    this->webServer->on("/", HTTP_GET, [this] (AsyncWebServerRequest* request) {
        request->send(LittleFS, "/index.html", "text/html");
    });

    this->webServer->on("/app.js", HTTP_GET, [this] (AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/app.js.gz", "text/javascript");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    this->webServer->on("/app.css", HTTP_GET, [this] (AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/app.css.gz", "text/css");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    this->webServer->on("/api/status", HTTP_GET, [] (AsyncWebServerRequest* request) {
        request->send(200, "application/json", ljson_stringify((new LJsonObject)
            ->addChild("fragmentation", ESP.getHeapFragmentation())
            ->addChild("memory_free", (long)ESP.getFreeHeap())
            ->addChild("max_free_block", (long)ESP.getMaxFreeBlockSize())
        , true));
    });

    this->webServer->begin();
}

void WebServer::loop() {
}

LJsonAsyncWebServer* WebServer::getWebServer() {
    return this->webServer;
}

void WebServer::clean() {
    log_n("Clean Web Server");
    if (this->webServer) { 
        this->webServer->end();
        delete this->webServer;
        this->webServer = NULL;
    }
}
