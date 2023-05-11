/*
Connection between ESP32 and MPU6050
*/
// VCC	Power the sensor (3.3V or 5V)
// GND	Common GND
// SCL	SCL pin for I2C communication (GPIO 22)
// SDA	SDA pin for I2C communication (GPIO 21)
// XDA	Used to interface other I2C sensors with the MPU-6050
// XCL	Used to interface other I2C sensors with the MPU-6050
// AD0	Use this pin to change the I2C address
// INT	Interrupt pin – can be used to indicate that new measurement data is available
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "spiffs.h"




void mpu_init();
void mpu_get_event();
uint8_t Get_NVS_Data(String);
void NVS_Save_DATA(float time_T , float Gyroscope_X,float Gyroscope_Y,float Gyroscope_Z, float Accelerometer_X, float Accelerometer_Y, float Accelerometer_Z);