#include <sys/_stdint.h>
#include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include <math.h>
#include "Pixel.cpp"

#define PIXEL_COLOR_PER_INPUT   2
#define LED_COUNT   300

extern uint32_t colors[];
extern int rings[6][2];
extern const size_t NUM_COLORS;

class Spool {
  public:
    Pixel pixels[LED_COUNT];
    uint8_t pixelColors[LED_COUNT];
    CRGB (&leds)[LED_COUNT];

    String state;
    int pixelsCounter;
    int pixelsPerCircle;
    int maxCircles;
    int maxPixels;

    int queue;
    int i;

    Spool(String state, CRGB (&leds)[LED_COUNT]): state(state), leds(leds) {
      this->pixelsCounter = 0;
      this->pixelsPerCircle = 3 * PIXEL_COLOR_PER_INPUT * NUM_COLORS;
      this->maxCircles = 4;
      this->maxPixels = this->pixelsPerCircle * this->maxCircles;
      this->i = 0;

      int numRings = sizeof(rings) / sizeof(rings[0]);
      for (int i = 0; i < LED_COUNT; ++i) {
        for(int ring = 0; ring < numRings; ring++) {
          if (i >= rings[ring][0] && i < rings[ring][1]) {
            pixels[i] = Pixel(ring+1); 
          }
        }
      }
    }

    void switchState(String newState) {
      this->state = newState;
    }

    void addPixels() {
      this->queue += NUM_COLORS * PIXEL_COLOR_PER_INPUT;
    }

    void tick() {
      if (this->queue > 0) {
        this->queue--;
        uint8_t index = this->i / PIXEL_COLOR_PER_INPUT;
        if (index < NUM_COLORS) {
          unshift(pixelColors, LED_COUNT, index);
        }
        this->i = (this->i + 1) % (NUM_COLORS * PIXEL_COLOR_PER_INPUT);
      } else {
        unshift(pixelColors, LED_COUNT, 0);
      }

      if (pixelColors[LED_COUNT - 1] != 0x0 && this->pixelsCounter < this->maxPixels) {
        this->pixelsCounter++;
      }
    }

    void draw() {
      // swirling in pixels
      for (int i = 0; i < LED_COUNT; i++) {
        pixels[i].setColor(pixelColors[i]);
        pixels[i].draw(leds[i]); 
      }

      int filledRings = floor(this->pixelsCounter / this->pixelsPerCircle);
      int pixelsInCurrentRing = this->pixelsCounter % this->pixelsPerCircle;

      for (int i = 0; i < LED_COUNT; ++i) {
        Pixel &pixel = pixels[i];
        if (pixel.ringNumber <= filledRings && pixel.ringNumber > 0) {
          pixel.setColor(1);
          pixel.draw(leds[i]);
        } 
        // else if (pixel.ringNumber == filledRings + 1) {
        //   int opacity = map(pixelsInCurrentRing, 0, pixelsPerCircle, 0, 100);
        //   float brightnessFactor = opacity / 100;

        //   uint8_t red = ((colorFinal >> 16) & 0xFF) * brightnessFactor;
        //   uint8_t green = ((colorFinal >> 8) & 0xFF) * brightnessFactor;
        //   uint8_t blue = (colorFinal & 0xFF) * brightnessFactor;

        //   uint32_t dimmedColor = (red << 16) | (green << 8) | blue;
        //   pixel.setColor(dimmedColor);
        //   pixel.draw();
        // }
      }
    }

    void updateCircles() {
      // Check if the last pixel in the array has reached the center of the spool
      int maxPixels = this->pixelsPerCircle * this->maxCircles;
      if (
        this->pixels[LED_COUNT - 1].colorIdx > 0 &&
        this->pixelsCounter < maxPixels
      ) {
        this->pixelsCounter++;
      }
    }

    void unshift(uint8_t arr[], int size, uint8_t newValue) {
      for (int j = size - 1; j > 0; j--) {
          arr[j] = arr[j - 1];
      }
      arr[0] = newValue;
    }
};
