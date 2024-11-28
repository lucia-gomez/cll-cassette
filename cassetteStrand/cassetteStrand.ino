// #include <Adafruit_NeoPixel.h>
// #ifdef __AVR__
//  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
// #endif
#include <FastLED.h>
#include "Cassette.cpp"

#define LED_PIN_LEFT_SPOOL      32
#define LED_PIN_RIGHT_SPOOL     27
#define LED_PIN_INFINITY        33

#define SPOOL_LED_COUNT         300
#define INFINITY_LED_COUNT      150

// int buttonAddPin = 15;
int buttonState = LOW;
int lastButtonState = LOW;

// LED strips
CRGB spoolLeftLeds[SPOOL_LED_COUNT];
CRGB spoolRightLeds[SPOOL_LED_COUNT];
CRGB infinityLeds[INFINITY_LED_COUNT];

// Cassette model
Cassette cassette(spoolLeftLeds, spoolRightLeds, infinityLeds);

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
  {191, SPOOL_LED_COUNT - 65},
};

unsigned long intervalLeft, intervalRight, timeoutLeft, timeoutRight;
unsigned long lastTick;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812, LED_PIN_LEFT_SPOOL, GRB>(spoolLeftLeds, SPOOL_LED_COUNT);
  FastLED.addLeds<WS2812, LED_PIN_RIGHT_SPOOL, GRB>(spoolRightLeds, SPOOL_LED_COUNT);
  FastLED.addLeds<WS2812, LED_PIN_INFINITY, GRB>(infinityLeds, INFINITY_LED_COUNT);

  FastLED.setBrightness(255);

  // if (cassette.state == "start" || cassette.state == "halfFull") {
    // scheduleInputLeft();
    // scheduleInputRight();
  // }

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
}

void scheduleInputLeft() {
  intervalLeft = random(2000, 5000);
  timeoutLeft = millis() + intervalLeft;
}

void scheduleInputRight() {
  intervalRight = random(5000, 15000);
  timeoutRight = millis() + intervalRight;
}
