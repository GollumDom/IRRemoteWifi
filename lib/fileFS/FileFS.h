#ifndef fileFS_FileFS_h
#define fileFS_FileFS_h

#include <helper.h>
#include <LittleFS.h>

class FileFS {
    public:
        FileFS();
        ~FileFS();
        void init(std::function<void()> cbError = NULL);
        void loop();
        void format();
        bool filePutContent(String file, String content);
        bool fileAppendContent(String file, String content);
        bool fileReadContent(String file, String& content);
};

#endif
