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

typedef struct struct_message {
    // char a[32]; // different available data structure
    // int b;
    // float c;
   bool r;
   bool b;
   bool y;
   bool g;
} struct_message;

// Create a struct_message called myData
struct_message myData;
const int RedPin = 33; 
const int BluePin = 26; 
const int YellowPin = 25; 
const int GreenPin = 32; 

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);
  // Serial.print("Int: ");
  // Serial.println(myData.b);
  // Serial.print("Float: ");
  // Serial.println(myData.c);
  Serial.print("REDBool: ");
  Serial.println(myData.r);
  Serial.println();
  Serial.print("BLUEBool: ");
  Serial.println(myData.b);
  Serial.println();
  Serial.print("YELLOWBool: ");
  Serial.println(myData.y);
  Serial.println();
  Serial.print("GREENBool: ");
  Serial.println(myData.g);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(YellowPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {
if(myData.r == 1){
  digitalWrite(RedPin, HIGH);
  Serial.println("RED");
}
if(myData.r != 1){
  digitalWrite(RedPin, LOW);
}
if(myData.b == 1){
  digitalWrite(BluePin, HIGH);
  Serial.println("BLUE");
}
if(myData.b != 1){
  digitalWrite(BluePin, LOW);
}
if(myData.y == 1){
  digitalWrite(YellowPin, HIGH);
  Serial.println("YELLOW");
}
if(myData.y != 1){
  digitalWrite(YellowPin, LOW);
}
if(myData.g == 1){
  digitalWrite(GreenPin, HIGH);
  Serial.println("GREEN");
}
if(myData.g != 1){
  digitalWrite(GreenPin, LOW);
}

}
