#include <Arduino.h>
#include <FastLED.h>

#include "Spool.cpp"
#include "Infinity.cpp"

// 242 for spiral + 80 * 6 for leg
#define SPOOL_LED_COUNT   722
#define INFINITY_LED_COUNT 150

class Cassette {
  public:
    String state;
    Spool spoolLeft;
    Spool spoolRight;
    Infinity infinity;

    Cassette(
      CRGB (&spoolLeftLeds)[SPOOL_LED_COUNT], 
      CRGB (&spoolRightLeds)[SPOOL_LED_COUNT],
      CRGB (&infinityLeds)[INFINITY_LED_COUNT]
    ) : 
      state("start"), 
      spoolLeft(state, spoolLeftLeds), 
      spoolRight(state, spoolRightLeds), 
      infinity(state, infinityLeds) {
        this->switchState("start");
      }

    void switchState(String newState) {
      this->state = newState;
      spoolLeft.switchState(newState); 
      spoolRight.switchState(newState); 
      infinity.switchState(newState);
    }

    void tick() {
      spoolLeft.tick();
      spoolRight.tick();
      infinity.tick();
    }

    void draw() {
      spoolLeft.draw();
      spoolRight.draw();
      infinity.draw();
    }
};
