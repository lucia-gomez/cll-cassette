// #include <Adafruit_NeoPixel.h>
// #ifdef __AVR__
//  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
// #endif
#include <FastLED.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Cassette.cpp"

#define LED_PIN_LEFT_SPOOL      32
#define LED_PIN_RIGHT_SPOOL     15
#define LED_PIN_INFINITY        33
#define LED_PIN_OUTLINE         27

#define SPOOL_LED_COUNT         242
#define INFINITY_LED_COUNT      150
#define LEG_LED_COUNT           80
#define LEG_LED_COLUMNS         6
#define OUTLINE_LED_COUNT       200 // overestimate

// int buttonAddPin = 15;
int buttonState = LOW;
int lastButtonState = LOW;

// LED strips
CRGB spoolLeftLeds[SPOOL_LED_COUNT + LEG_LED_COUNT * LEG_LED_COLUMNS];
CRGB spoolRightLeds[SPOOL_LED_COUNT + LEG_LED_COUNT * LEG_LED_COLUMNS];
CRGB infinityLeds[INFINITY_LED_COUNT];
CRGB outlineLeds[OUTLINE_LED_COUNT];

// Cassette model
Cassette cassette(spoolLeftLeds, spoolRightLeds, infinityLeds, outlineLeds);

// Incoming pixel colors;
uint32_t colors[] = {
    0x000000, // off
    0x6600ff, // final purple color
    0x5f03ff, 
    0xb700ff,
    0xff00bb,
    0xFFA9FF
};
const size_t NUM_COLORS = sizeof(colors) / sizeof(colors[0]); 

int rings[5][2] = {
  {0, 36},
  {36, 81},
  {81, 133},
  {133,191},
  {191, SPOOL_LED_COUNT - 20},
};

unsigned long intervalLeft, intervalRight, timeoutLeft, timeoutRight;
unsigned long lastTick;

float inputRate;
const float INPUT_RATE_MIN = 1.0; // ~45min duration
const float INPUT_RATE_MAX = 10.0; // ~4.5min duration

typedef struct struct_message {
  bool on;
  bool playing;
  bool unlocked;
  float rate;
} struct_message;

struct_message receivedData = {true, true, false, 0.0};
struct_message previousData = {true, true, false, 0.0};
bool messageReceived = false;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  previousData = receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  messageReceived = true;

  Serial.print("ON: ");
  Serial.println(receivedData.on);
  Serial.println();
  Serial.print("PLAYING: ");
  Serial.println(receivedData.playing);
  Serial.println();
  Serial.print("UNLOCKED: ");
  Serial.println(receivedData.unlocked);
  Serial.println();
  Serial.print("INPUT RATE: ");
  Serial.println(receivedData.rate);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812, LED_PIN_LEFT_SPOOL, GRB>(spoolLeftLeds, SPOOL_LED_COUNT + LEG_LED_COUNT * LEG_LED_COLUMNS);
  FastLED.addLeds<WS2812, LED_PIN_RIGHT_SPOOL, GRB>(spoolRightLeds, SPOOL_LED_COUNT + LEG_LED_COUNT * LEG_LED_COLUMNS);
  FastLED.addLeds<WS2812, LED_PIN_INFINITY, GRB>(infinityLeds, INFINITY_LED_COUNT);
  FastLED.addLeds<WS2812, LED_PIN_OUTLINE, GRB>(outlineLeds, OUTLINE_LED_COUNT);

  FastLED.setBrightness(128);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  inputRate = 10.0;
  if (cassette.state == "start") {
    scheduleInputLeft();
    scheduleInputRight();
  }
}

void loop() {
  if (messageReceived) {
    if (previousData.on != receivedData.on) {
      if (receivedData.on) { // off -> on
        Serial.println("State ON");
        cassette.switchState("start");
        scheduleInputLeft();
        scheduleInputRight();
      } else { // on -> off
        Serial.println("State OFF");
        cassette.switchState("off");
        FastLED.show();
      }
    }
    messageReceived = false;
  }

  //  || !receivedData.playing
  if (cassette.state == "off") {
    return; // do nothing
  }

  // if (previousData.unlocked != receivedData.unlocked) {
  //   if(receivedData.unlocked) { // filling up -> unlocked
  //     cassette.switchState("unlock");
  //   } else { // unlock -> filling up
  //     cassette.switchState("start");
  //   }
  // }

  if (millis() >= timeoutLeft) {
    cassette.spoolLeft.addPixels();
    Serial.println("LEFT");
    scheduleInputLeft();
  }

  if (millis() >= timeoutRight) {
    cassette.spoolRight.addPixels();
    Serial.println("RIGHT");
    scheduleInputRight();
  }

  // TODO speed controller?
  if (millis() - lastTick >= 50) {
    cassette.tick();
    lastTick = millis();
  }

  cassette.draw();
  FastLED.show();
}

void scheduleInputLeft() {
  intervalLeft = random(5000, 10000);
  timeoutLeft = millis() + intervalLeft;
}

void scheduleInputRight() {
  intervalRight = random(5000, 15000);
  timeoutRight = millis() + intervalRight;
}

void debugLegs() {
  for(int i = 0; i < 242; i++) {
    spoolLeftLeds[i] = CRGB::Green;
    spoolRightLeds[i] = CRGB::Green;
  }
  for(int i = 0; i < LEG_LED_COLUMNS; i++) {
    for(int j = 0; j < LEG_LED_COUNT; j++) {
      int color;
      if (i == 0) {
        color = CRGB::Blue;
      } else if (i == 1) {
        color = CRGB::Cyan;
      }
      else if (i == 2) {
        color = CRGB::Red;
      }
      else if (i == 3) {
        color = CRGB::Pink;
      }
      else if (i == 4) {
        color = CRGB::Green;
      }
      else if (i == 5) {
        color = CRGB::Red;
      }
      spoolLeftLeds[i * LEG_LED_COUNT + 242 + j] = color;
    }
  }
}
