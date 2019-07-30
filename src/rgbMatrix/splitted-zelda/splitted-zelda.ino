#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#include "zelda.h"

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
  int part = 1;
  int timeBetweenAnimations = 50000;
  showAnimation(part, zelda, zeldaFrames, 250);
  delay(timeBetweenAnimations);
}


void showAnimation(int part, const long animation[][4096], int nFrames, int frameSpeed) {
  for(int frame = 0; frame < nFrames; frame++) {
    delay(frameSpeed);
    int pixelCount = 0;
    if (part == 2) { pixelCount = 32*64; }
    for (int y = 0; y < 32; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t colorHex = pgm_read_dword(&animation[frame][pixelCount]);
        uint8_t r = (uint8_t)(colorHex >> 16);
        uint8_t g = (uint8_t)(colorHex >> 8);
        uint8_t b = (uint8_t)(colorHex >> 0);
        matrix.drawPixel(x, y, matrix.Color888(r, g, b, true));
        pixelCount ++;
      }
    }    
  }
}
