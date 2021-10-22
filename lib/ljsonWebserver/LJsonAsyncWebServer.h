#ifndef LJsonAsyncWebServer_h
#define LJsonAsyncWebServer_h

#include <ESPAsyncWebServer.h>
#include <LJson.h>

struct WebUpload {
    String filename;
    String data;
};

typedef std::function<void(AsyncWebServerRequest *request, String body)> ArGroupedBodyHandlerFunction;
typedef std::function<void(AsyncWebServerRequest *request, WebUpload file)> ArGroupedUploadHandlerFunction;
typedef std::function<void(AsyncWebServerRequest *request, String body, WebUpload file)> ArGroupedBodyAndUploadHandlerFunction;

class LJsonAsyncWebServer: public AsyncWebServer {
    public:
        LJsonAsyncWebServer(uint16_t port = 80);
        AsyncCallbackWebHandler& onBody(const char* uri, WebRequestMethodComposite method, ArGroupedBodyHandlerFunction onBody);
        AsyncCallbackWebHandler& onUpload(const char* uri, WebRequestMethodComposite method, ArGroupedUploadHandlerFunction onUpload);
        AsyncCallbackWebHandler& onBodyAndUpload(const char* uri, WebRequestMethodComposite method, ArGroupedBodyAndUploadHandlerFunction onBodyAndUpload);

        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, LJsonNode* json)> callback);
        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, LJsonNode* json, String body)> callback);

        template<typename T, typename std::enable_if<std::is_base_of<LDenormalized, T>::value>::type* = nullptr>
        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, T* bindedObject)> callback) {
            return this->onJson<T>(uri, method, [callback](AsyncWebServerRequest* request, T* bindedObject, String body) {
                callback(request, bindedObject);
            });
        }

        template<typename T, typename std::enable_if<std::is_base_of<LDenormalized, T>::value>::type* = nullptr>
        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, T* bindedObject, String body)> callback) {

            return this->onBody(uri, method, [callback](AsyncWebServerRequest* request, String body) {

                T* obj = new T();
                if (ljson_parse(body, obj)) {
                    Serial.println("cool");
                    callback(request, obj, body);
                    return;
                } else {
                    delete obj;
                }
                request->send(400, "application/json", "{\"error\":\"Parsing error\"}");
            });
            
        }
    private:
        std::string body = "";
        String filename = "";
        std::string upload = "";
};

#endif