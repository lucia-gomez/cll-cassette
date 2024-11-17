// #include <Adafruit_NeoPixel.h>
// #ifdef __AVR__
//  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
// #endif
#include <FastLED.h>
#include "Cassette.cpp"

#define LED_PIN_LEFT_SPOOL     32
#define LED_PIN_RIGHT_SPOOL     27
#define LED_COUNT   300
// #define BRIGHTNESS 50 // 0-255

// int buttonAddPin = 15;
int buttonState = LOW;
int lastButtonState = LOW;

// LED strips
CRGB spoolLeftLeds[LED_COUNT];
CRGB spoolRightLeds[LED_COUNT];

// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Cassette model
Cassette cassette(spoolLeftLeds, spoolRightLeds);

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

int rings[6][2] = {
  {200, 1200},
  {144, 200},
  {115,144},
  {80, 115},
  {40, 80},
  {0, 40},
};

unsigned long intervalLeft, intervalRight, timeoutLeft, timeoutRight;
unsigned long lastTick;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN_LEFT_SPOOL, GRB>(spoolLeftLeds, LED_COUNT);
  FastLED.addLeds<WS2812, LED_PIN_RIGHT_SPOOL, GRB>(spoolRightLeds, LED_COUNT);

  FastLED.setBrightness(64);
  // strip.setBrightness(BRIGHTNESS);
  // strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // strip.show();  // Turn OFF all pixels ASAP

  if (cassette.state == "start" || cassette.state == "halfFull") {
    // scheduleInputLeft();
    // scheduleInputRight();
  }

  // pinMode(buttonAddPin, INPUT);
}

void loop() {
  // int reading = digitalRead(buttonAddPin);
  // if (reading == HIGH && lastButtonState == LOW) {
  //   Serial.println("Button pressed!");
  //   cassette.spoolLeft.addPixels();
  // }
  // lastButtonState = reading;

  if (millis() >= timeoutLeft && cassette.state != "manual") {
    cassette.spoolLeft.addPixels();
    Serial.println("LEFT");
    scheduleInputLeft();
  }

  if (millis() >= timeoutRight && cassette.state != "manual") {
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
  // strip.show();
}

void scheduleInputLeft() {
  intervalLeft = random(2000, 5000);
  timeoutLeft = millis() + intervalLeft;
}

void scheduleInputRight() {
  intervalRight = random(5000, 15000);
  timeoutRight = millis() + intervalRight;
}
