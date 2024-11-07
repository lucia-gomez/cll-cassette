#include <sys/_stdint.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "Pixel.cpp"

#define PIXEL_COLOR_PER_INPUT   2

extern uint32_t colorFinal;
extern uint32_t colorInput[];
extern int rings[4][2];
extern const size_t NUM_COLOR_INPUTS;

class Spool {
  public:
    Pixel pixels[144];
    uint32_t pixelColors[144];

    String state;
    int pixelsCounter;
    int pixelsPerCircle;
    int maxCircles;

    int queue;
    int i;

    Spool(String state) {
      this->state = state;
      this->pixelsCounter = 0;
      this->pixelsPerCircle = 3 * PIXEL_COLOR_PER_INPUT * NUM_COLOR_INPUTS;
      this->maxCircles = 4;
      this->i = 0;

      int numRings = sizeof(rings) / sizeof(rings[0]);
      for (int i = 0; i < 144; ++i) {
        for(int ring = 0; ring < numRings; ring++) {
          if (i >= rings[ring][0] && i < rings[ring][1]) {
            pixels[i] = Pixel(i, ring+1); 
          }
        }
      }
    }

    void switchState(String newState) {
      this->state = newState;
    }

    void addPixels() {
      this->queue += NUM_COLOR_INPUTS * PIXEL_COLOR_PER_INPUT;
    }

    void tick() {
      if (this->queue > 0) {
        this->queue--;
        int index = this->i / PIXEL_COLOR_PER_INPUT;
        if (index < NUM_COLOR_INPUTS) {
          uint32_t c = colorInput[index];
          // pixelColors[0] = c;
          unshift(pixelColors, 144, c);
        }
        this->i = (this->i + 1) % (NUM_COLOR_INPUTS * PIXEL_COLOR_PER_INPUT);
         this->updateCircles();
      } else {
        unshift(pixelColors, 144, 0);
      }
      Serial.println(this->pixelsCounter);
    }

    void draw() {
      // swirling in pixels
      for (int i = 0; i < 144; i++) {
        pixels[i].setColor(pixelColors[i]);
        pixels[i].draw(); 
      }

     

      int filledRings = floor(this->pixelsCounter / this->pixelsPerCircle);
      int pixelsInCurrentRing = this->pixelsCounter % this->pixelsPerCircle;
      // Serial.print(filledRings);
      // Serial.print(", ");
      // Serial.println(pixelsInCurrentRing);

      for (int i = 0; i < 144; ++i) {
        Pixel &pixel = pixels[i];
        if (pixel.ringNumber <= filledRings && pixel.ringNumber > 0) {
          pixel.setColor(colorFinal);
          pixel.draw();
        } else if (pixel.ringNumber == filledRings + 1) {
          int opacity = map(pixelsInCurrentRing, 0, pixelsPerCircle, 0, 255);
          float brightnessFactor = opacity / 255.0;

          uint8_t red = ((colorFinal >> 16) & 0xFF) * brightnessFactor;
          uint8_t green = ((colorFinal >> 8) & 0xFF) * brightnessFactor;
          uint8_t blue = (colorFinal & 0xFF) * brightnessFactor;

          uint32_t dimmedColor = (red << 16) | (green << 8) | blue;
          pixel.setColor(dimmedColor);
          pixel.draw();
        }
      }
    }

    void updateCircles() {
      // Check if the last pixel in the array has reached the center of the spool
      int maxPixels = this->pixelsPerCircle * this->maxCircles;
      if (
        this->pixels[144 - 1].color > 0x000000 &&
        this->pixelsCounter < maxPixels
      ) {
        this->pixelsCounter++;
      }
    }

    void unshift(uint32_t arr[], int size, uint32_t newValue) {
      for (int j = size - 1; j > 0; j--) {
          arr[j] = arr[j - 1];
      }
      arr[0] = newValue;
    }
};
