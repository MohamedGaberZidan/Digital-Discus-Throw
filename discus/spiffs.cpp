//#include "FS.h"
//#include "SPIFFS.h"
#include "String.h"
#include "spiffs.h"
#include "mpu6050.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */


int x = 0;
extern bool max_size_flag= false;
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    uint8_t error_flag ;
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    Serial.println("- read from file:");
    uint32_t file_line = 0;
    while(file.available()){
        String s = file.readStringUntil('\n');
        //Serial.println(s);
        //Serial.write(file.read());
        error_flag= Get_NVS_Data(s);
        // if(error_flag == NETWORK_FAILURE)
        //     return;
        file_line++;
    }
    file.close();
    //deleting the file after reading all the data from it
    deleteFile(SPIFFS, "/saved_data.txt");
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    
    // if(file.size()>0){
    // //    saved_data_flag = SAVED_DATA;
    //    saved_data_flag = 1;
    // }

    // if(file.print(message)){
    //     Serial.println("- file written");
    // } else {
    //     Serial.println("- write failed");
    // }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }


    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    size_t len = file.size();
    Serial.println("Our size is "+ String(len));
    if(len>=640000UL)// spiffs max size after adjusting the partition table
        max_size_flag = true;
   file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){

    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}
void SPIFFS_INIT(void){
       if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
       Serial.println("SPIFFS Mount Failed");
       return;
   }
        listDir(SPIFFS, "/", 0);
        //deleteFile(SPIFFS, "/save_data.txt");
        // delay(1000);
         writeFile(SPIFFS, "/saved_data.txt","");
}  
