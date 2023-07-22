/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include "wifi.h"
#include "mpu6050.h"

extern volatile bool start_saving_data;
bool volatile start_sending_data = false;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[55]; // will be used for sending the data
  bool send_data; // will be used for requesting the data
  bool save_data; // will be used for requesting stating saving the data and stopping it
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;
String incoming_string;
bool incoming_cmd = false;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  incoming_string = myData.a;

  // checking if the transmitter ready for getting the data 
  if(myData.send_data){
    start_sending_data = true;
//    readFile(SPIFFS, "/saved_data.txt");
//    //Serial.println("start sending data");
  }
  // checking starting or stopping saving data
  if(myData.save_data)
    start_saving_data= true;
  else
    start_saving_data= false;

}


void setup_wifi()
{
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

}
 
void send(String data) {
  // Set values to send
  strcpy(myData.a, data.c_str());
  myData.send_data = false;
  myData.save_data = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }
  delay(100);
}
