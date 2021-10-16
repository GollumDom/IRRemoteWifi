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


        template<typename T, typename std::enable_if<std::is_base_of<LDenormalized, T>::value>::type* = nullptr>
        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, T* bindedObject)> callback) {
            return this->onJson<T>(uri, method, [callback](AsyncWebServerRequest* request, T* bindedObject, String body) {
                callback(request, bindedObject);
            });
        }

        template<typename T, typename std::enable_if<std::is_base_of<LDenormalized, T>::value>::type* = nullptr>
        AsyncCallbackWebHandler& onJson(const char* uri, WebRequestMethodComposite method, std::function<void(AsyncWebServerRequest* request, T* bindedObject, String body)> callback) {

            return this->onBody(uri, method, [callback](AsyncWebServerRequest* request, String body) {
                
                LJsonNode* json = ljson_parse(body);
                
                if (json) {
                    T* obj = new T();
                    if (obj->fromJson(json)) {
                        delete json;
                        callback(request, obj, body);
                        return;
                    } else {
                        delete json;
                        delete obj;
                    }
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