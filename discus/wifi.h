#ifndef WIFI_H
#define WIFI_H



// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
void setup_wifi();
void send(String);


#endif