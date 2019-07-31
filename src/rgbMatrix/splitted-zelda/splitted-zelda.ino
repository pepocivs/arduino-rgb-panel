#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#include "zeldaA.h"
#include "bigtriforceA.h"


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
  int timeBetweenAnimations = 5000;
  int timeBetweenFrames = 220;
  // showAnimation(zelda0);
  // delay(timeBetweenAnimations);
  showAnimation(bigtriforce0);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce1);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce2);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce3);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce4);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce5);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce6);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce7);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce8);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce9);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce10);
  delay(timeBetweenFrames);
  showAnimation(bigtriforce11);
  delay(timeBetweenFrames);
  // delay(timeBetweenAnimations);
}

void showAnimation(const long animation[2048]) {
  int pixelCount = 0;
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      uint32_t colorHex = pgm_read_dword(&animation[pixelCount]);
      uint8_t r = (uint8_t)(colorHex >> 16);
      uint8_t g = (uint8_t)(colorHex >> 8);
      uint8_t b = (uint8_t)(colorHex >> 0);
      matrix.drawPixel(x, y, matrix.Color888(r, g, b, true));
      pixelCount ++;
    }
  }
}
