
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xC4, 0xDE, 0xE2, 0x9D, 0x47, 0xA0};
uint8_t broadcastAddress1[] = {0x14, 0x2B, 0x2F, 0xAE, 0xBE, 0x3C};
//14:2b:2f:ae:be:3c
// c4:de:e2:9d:47:a0 
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  // char a[32]; different data structure 
  // int b;
  // float c;
  bool r;
  bool b;
  bool y;
  bool g;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo1;
esp_now_peer_info_t peerInfo2;

int RedRead = 0;
int BlueRead = 0;
int YellowRead = 0;
int GreenRead = 0;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
 snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
const int RedPin = 33; 
const int BluePin = 34; 
const int YellowPin = 35; 
const int GreenPin = 32; 



void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(RedPin, INPUT);
  pinMode(BluePin, INPUT);
  pinMode(YellowPin, INPUT);
  pinMode(GreenPin, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer



  memcpy(peerInfo1.peer_addr, broadcastAddress, 6);
  peerInfo1.channel = 0;
  peerInfo1.encrypt = false;
  if (esp_now_add_peer(&peerInfo1) != ESP_OK) {
   Serial.println("Failed to add peer 1");
   return;
  }
  memcpy(peerInfo2.peer_addr, broadcastAddress1, 6);
  peerInfo2.channel = 0;
  peerInfo2.encrypt = false;
  if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
   Serial.println("Failed to add peer 2");
   return;
 }
  
   

}
 
void loop() {
  // Set values to send
  // strcpy(myData.a, "THIS IS A CHAR");
  // myData.b = random(1,20);
  // myData.c = 1.2;
  // myData.d = false;
  RedRead = digitalRead(RedPin);
  BlueRead = digitalRead(BluePin);
  YellowRead = digitalRead(YellowPin);
  GreenRead = digitalRead(GreenPin);

  if(RedRead == HIGH){
  myData.r = true;
  Serial.println("red");
  }
  if(BlueRead == HIGH){
  myData.b = true;
  Serial.println("blue");
  }
  if(YellowRead == HIGH){
  myData.y = true;
  Serial.println("yellow");
  }
  if(GreenRead == HIGH){
  myData.g = true;
  Serial.println("green");
  }
   if(RedRead == LOW){
  myData.r = false;
  
  }
  if(BlueRead == LOW){
  myData.b = false;
  
  }
  if(YellowRead == LOW){
  myData.y = false;
 
  }
  if(GreenRead == LOW){
  myData.g = false;
  
  }
   
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else if (result1 == ESP_OK) {
    Serial.println("1Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
}