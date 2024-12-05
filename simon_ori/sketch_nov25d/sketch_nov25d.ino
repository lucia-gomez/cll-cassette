#include "pitches.h"
#include <FastLED.h>

#define NUM_LEDS    1
#define LED_PIN     27
const int buttonPin = 26;
#define SPEAKER_PIN 7

CRGB leds[NUM_LEDS];

// Game settings
#define MAX_GAME_LENGTH 100
byte gameSequence[MAX_GAME_LENGTH] = {0};
byte gameIndex = 0;

const int gameTone = NOTE_C4; // Single tone for one LED

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  randomSeed(analogRead(25)); // Prime random number generator
}

void lightLedAndPlayTone() {
  leds[0] = CRGB::Red;  // Light up LED
  FastLED.show();
  tone(SPEAKER_PIN, gameTone); // Play tone
  delay(300);
  leds[0] = CRGB::Black;  // Turn off LED
  FastLED.show();
  noTone(SPEAKER_PIN);
}

void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    lightLedAndPlayTone();
    delay(500); // Pause between flashes
  }
}

bool checkUserInput() {
  for (int i = 0; i < gameIndex; i++) {
    while (digitalRead(buttonPin) == HIGH) {
      delay(10); // Wait for button press
      Serial.println("HIGH");
    }
    lightLedAndPlayTone();
    Serial.println("low");
    delay(500); // Debounce
  }
  return true;
}

void gameOver() {
  Serial.println("Game Over!");
  gameIndex = 0;
  tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
  noTone(SPEAKER_PIN);
}

void loop() {
  // Add a random step to the sequence
  
  gameSequence[gameIndex] = random(0, 1);
  gameIndex++;

  playSequence();

  if (!checkUserInput()) {
    gameOver();
  } else {
    Serial.print("Level up! Current score: ");
    Serial.println(gameIndex);
    delay(1000);
  }
}
