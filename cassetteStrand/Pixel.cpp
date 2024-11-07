#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;

class Pixel {
  public:
    int i;
    int ringNumber;
    uint32_t color; // RGB

    // default
    Pixel() {
      this->i = 0;
      this->ringNumber = 0;
      this->color = 0x000000;
    }

    Pixel(int i, int ringNumber = 0) {
      this->i = i;
      this->ringNumber = ringNumber;
      this->color = 0x000000;
    }

    void setColor(uint32_t c) {
      this->color = c;
    }

    void draw() {
      strip.setPixelColor(this->i, this->color); 
    }
};
