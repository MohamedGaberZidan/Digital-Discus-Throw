#include "mpu6050.h"
#include "wifi.h"
#include "spiffs.h"
bool volatile start_saving_data;
extern volatile bool start_sending_data ;
uint64_t time_start_t = 0;
bool cycle_started = false;
void setup()
{
    // Serial.begin(115200);
    // Setup wifi
    setup_wifi();
    delay(1000);
    // Setup MPU
    mpu_init();
    delay(1000);
    // Setup Spiffs
    SPIFFS_INIT();
    
}


void loop()
{
   if(start_sending_data){
    readFile(SPIFFS, "/saved_data.txt");

    }
    if(start_saving_data){
        if(!cycle_started){
            time_start_t = millis();
            cycle_started = true;}
        mpu_get_event();
    }
    else{
        cycle_started =false;
    }

}
