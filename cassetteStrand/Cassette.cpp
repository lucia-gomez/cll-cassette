#include <Arduino.h>
#include <FastLED.h>

#include "Spool.cpp"

#define LED_COUNT   300

class Cassette {
  public:
    String state;
    Spool spoolLeft;
    Spool spoolRight;

    Cassette(
      CRGB (&spoolLeftLeds)[LED_COUNT], 
      CRGB (&spoolRightLeds)[LED_COUNT]
    ) : 
      state("start"), 
      spoolLeft(state, spoolLeftLeds), 
      spoolRight(state, spoolRightLeds) {}

    void switchState(String newState) {
      this->state = newState;
      spoolLeft.switchState(newState); 
      spoolRight.switchState(newState); 
    }

    void tick() {
      spoolLeft.tick();
      spoolRight.tick();
    }

    void draw() {
      spoolLeft.draw();
      spoolRight.draw();
    }
};
