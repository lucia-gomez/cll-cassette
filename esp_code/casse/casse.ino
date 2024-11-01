#include <esp_now.h>
#include <WiFi.h>
const int butpin1 = 13;
const int butpin2 = 12;

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

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);
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
  Serial.println("P");
}

if(myData.b == 1){
  Serial.println("Q");
}

if(myData.y == 1){
 Serial.println("W");
}

if(myData.g == 1){
  Serial.println("E");
}


  
  delay(1000); 
}