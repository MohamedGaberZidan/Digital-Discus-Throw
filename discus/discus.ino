#include "mpu6050.h"
#include "wifi.h"
#include "spiffs.h"
bool start_saving_data;
uint64_t time_start_t = 0;
void setup()
{
    Serial.begin(115200);
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
    while(start_saving_data){
        time_start_t = millis();
        mpu_get_event();
    }
}