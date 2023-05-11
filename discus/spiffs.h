#ifndef _SPIFFS_H_
#define _SPIFFS_H_
#include <SPIFFS.h>
#include "FS.h"
#define FORMAT_SPIFFS_IF_FAILED true
namespace fs
{

class SPIFFSFS : public FS
{
public:
    SPIFFSFS();
    ~SPIFFSFS();
    bool begin(bool formatOnFail=false, const char * basePath="/spiffs", uint8_t maxOpenFiles=10, const char * partitionLabel=NULL);
    bool format();
    size_t totalBytes();
    size_t usedBytes();
    void end();
    
private:
    char * partitionLabel_;
};

}

extern fs::SPIFFSFS SPIFFS;
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readFile(fs::FS &fs, const char * path);
void appendFile(fs::FS &fs, const char * path, const char * message);
void writeFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);
void SPIFFS_INIT(void);
#endif