
#include <esp_now.h>
#include <WiFi.h>
#include "master_esp32.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char a[32]; // will be used for sending the data
  bool send_data; // will be used for requesting the data
  bool save_data; // will be used for requesting stating saving the data and stopping it
} struct_message;

// Create a struct_message called myData
struct_message myData;
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
//  Serial.print("ID: ");
  Serial.println(myData.a);
//   Serial.println(myData.send_data);
//   Serial.println(myData.save_data);


}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
    if(Serial.available())
    {
        String CMD = Serial.readString();
        if(CMD == "Start")
            send(false,true);
        else if(CMD == "Stop")
            send(false,false);
        else if(CMD == "Send")
            send(true,false);
        
    }

}
void send(bool send_data_CMD,bool save_data_CMD) {
  // Set values to send
  strcpy(myData.a, "" );
  myData.send_data = send_data_CMD;
  myData.save_data = save_data_CMD;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}