#include "FileFS.h"

FileFS::FileFS() {
}

FileFS::~FileFS() {
}

void listAllFilesInDir(String dir_path)
{
	Dir dir = LittleFS.openDir(dir_path);
    // print directory names
    log_l("    - D: "); log_n(dir_path);

	while(dir.next()) {
		if (dir.isFile()) {
			// print file names
			log_l("    - F: "); log_n(dir_path + dir.fileName());
		}
		if (dir.isDirectory()) {
			// recursive file listing inside new directory
			listAllFilesInDir(dir_path + dir.fileName() + "/");
		}
	}
}

void FileFS::init(std::function<void()> cbError) {
    log_n("Initialize SPIFFS/LittleFS");
    if (!LittleFS.begin()) {
        log_n("ERROR: SPIFFS/LittleFS failed!");
        if (cbError) {
            cbError();
        }
    }

    log_n("SPIFFS/LittleFS list files on system:");
    listAllFilesInDir("/");
}

void FileFS::loop() {
}

void FileFS::format() {
    log_n("SPIFFS/LittleFS format.");
    LittleFS.format();
}

bool FileFS::filePutContent(String path, String content) {
    File file = LittleFS.open(path, "w");
    if (!file) {
        return false;
    }
    file.print(content);
    file.close();
    return true;
}

bool FileFS::fileAppendContent(String path, String content) {
    File file = LittleFS.open(path, "a");
    if (!file) {
        return false;
    }
    file.print(content);
    file.close();
    return true;
}

bool FileFS::fileReadContent(String path, String& content) {
    File file = LittleFS.open(path, "r");
    if (!file) {
        return false;
    }
    char buf[file.size() + 1];
    uint64 i = 0;
    while(file.available()){
        buf[i] = (char)file.read();
        i++;
    }
    buf[i] = 0;
    content = buf;
    file.close();
    return true;
}
