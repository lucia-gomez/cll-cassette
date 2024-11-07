#include <Arduino.h>
#include "Spool.cpp"

class Cassette {
  public:
    String state;
    Spool spoolLeft;
    // Spool spoolRight;

    // initializer list
    Cassette() : state("start"), spoolLeft(state) {
    }

    void switchState(String newState) {
      this->state = newState;
      spoolLeft.switchState(newState); 
      // spoolRight.switchState(newState); 
    }

    void tick() {
      spoolLeft.tick();
    }

    void draw() {
      spoolLeft.draw();
    }
};
