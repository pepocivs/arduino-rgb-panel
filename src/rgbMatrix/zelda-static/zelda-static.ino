#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#include "linkA.h"

#define OE   9
#define LAT 10
#define CLK 11
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  matrix.begin();
}

void loop() {
  showAnimation(linkA, linkAFrames, linkAWidth, linkAHeight, 10);
}


void showAnimation(const uint8_t animation[][2048][3], int nFrames, int width, int height, int frameSpeed) {
  for(int frame = 0; frame < nFrames; frame++) {
    delay(frameSpeed);
    int pixelCount = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        uint8_t r = pgm_read_byte(&(animation[frame][pixelCount][0]));
        uint8_t g = pgm_read_byte(&(animation[frame][pixelCount][1]));
        uint8_t b = pgm_read_byte(&(animation[frame][pixelCount][2]));
        matrix.drawPixel(x, y, matrix.Color888(r, g, b, true));
        pixelCount ++;
      }
    }
  }
}
