#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x14, 0x2B, 0x2F, 0xAF, 0x0D, 0x18};

typedef struct struct_message {
  // char a[32]; different data structure 
  // int b;
  // float c;
  bool on;//r
  bool playing;//b
  bool unlocked;//y
  float rate;//g
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo1;

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

const int redButtonPin = 19;  // Red Button Pin
const int redLedPin = 33;     // Red LED Pin
bool redButtonState = HIGH;   // Red Button State

const int blkButtonPin = 26;  // Black Button Pin
const int blkLedPin = 27;    // Black LED Pin
bool blkButtonState = HIGH;  // Black Button State


const int blueButtonPin = 25;  // Blue Button Pin
const int blueLedPin = 12;    // Blue LED Pin
bool blueButtonState = HIGH; // Blue Button State

const int switchLedPin = 13; // Switch LED Pin

void setup() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo1.peer_addr, broadcastAddress, 6);
  peerInfo1.channel = 0;
  peerInfo1.encrypt = false;
  if (esp_now_add_peer(&peerInfo1) != ESP_OK) {
   Serial.println("Failed to add peer 1");
   return;
  }
  
  
  pinMode(switchLedPin, OUTPUT);
  digitalWrite(switchLedPin, HIGH);  // Turn on the switch LED and stay on for the duration of the program

  pinMode(redButtonPin, INPUT_PULLUP);    
  pinMode(blkButtonPin, INPUT_PULLUP);    
  pinMode(blueButtonPin, INPUT_PULLUP);  

  pinMode(redLedPin, OUTPUT);   
  pinMode(blkLedPin, OUTPUT);    
  pinMode(blueLedPin, OUTPUT);   

  digitalWrite(redLedPin, LOW);  
  digitalWrite(blkLedPin, LOW);  
  digitalWrite(blueLedPin, LOW);  

  Serial.begin(9600);        
}

void loop() {
  buttonToggle(redButtonPin);
  buttonToggle(blueButtonPin);
  buttonToggle(blkButtonPin);
}

void buttonToggle(int button) {
  // Get current button reading
  bool reading = digitalRead(button);
  
    // Handle each button
    if (button == redButtonPin) {
      if (reading != redButtonState) {
        redButtonState = reading;
        if (redButtonState == LOW) { 
          // Turn off the black and blue LEDs, and set the playing state to false
          digitalWrite(blkLedPin, LOW);
          digitalWrite(blueLedPin, LOW);
          myData.playing = false;
          myData.unlocked = false;

          // Toggle states for the red LED and the data structure
          digitalWrite(redLedPin, !digitalRead(redLedPin));
          myData.on = !myData.on;
        }
      }
    }
    else if (button == blkButtonPin) {
      if (reading != blkButtonState) {
        blkButtonState = reading;
        if (blkButtonState == LOW) {
          // Turn off the red and blue LEDs, and set the playing state to false
          digitalWrite(redLedPin, LOW);
          digitalWrite(blueLedPin, LOW);
          myData.on = false;
          myData.unlocked = false;

          // Toggle states for the black LED and playing state
          digitalWrite(blkLedPin, !digitalRead(blkLedPin));
          myData.playing = !myData.playing;
        }
      }
    }
    else if (button == blueButtonPin) {
      if (reading != blueButtonState) {
        blueButtonState = reading;
        if (blueButtonState == LOW) {
          // Turn off the red and black LEDs, and set the playing state to false
          digitalWrite(redLedPin, LOW);
          digitalWrite(blkLedPin, LOW);
          myData.on = false;
          myData.playing = false;

          // Toggle states for the blue LED and playing state
          digitalWrite(blueLedPin, !digitalRead(blueLedPin));
          myData.unlocked = !myData.unlocked;
        }
      }
    
  }
}