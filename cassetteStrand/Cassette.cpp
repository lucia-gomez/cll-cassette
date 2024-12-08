#include <Arduino.h>
#include <FastLED.h>

#include "Spool.cpp"
#include "Infinity.cpp"

// 242 for spiral + 80 * 6 for leg
#define SPOOL_LED_COUNT   722
#define INFINITY_LED_COUNT 150
#define OUTLINE_LED_COUNT 200

extern uint32_t colors[];

class Cassette {
  public:
    String state;
    Spool spoolLeft;
    Spool spoolRight;
    Infinity infinity;
    CRGB outlineLeds[OUTLINE_LED_COUNT];

    Cassette(
      CRGB (&spoolLeftLeds)[SPOOL_LED_COUNT], 
      CRGB (&spoolRightLeds)[SPOOL_LED_COUNT],
      CRGB (&infinityLeds)[INFINITY_LED_COUNT],
      CRGB (&outlineLeds)[OUTLINE_LED_COUNT]
    ) : 
      state("start"), 
      spoolLeft(state, spoolLeftLeds), 
      spoolRight(state, spoolRightLeds), 
      infinity(state, infinityLeds) {
        this->switchState("start");
        memcpy(this->outlineLeds, outlineLeds, sizeof(this->outlineLeds));
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

      if (this->state != "off") {
        for(int i = 0; i < OUTLINE_LED_COUNT; i++) {
          this->outlineLeds[i] = CRGB(colors[1]); // purple
        }
      }
    }
};
