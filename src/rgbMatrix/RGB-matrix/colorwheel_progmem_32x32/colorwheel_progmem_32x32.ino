// colorwheel_progmem demo for Adafruit RGBmatrixPanel library.
// Renders a nice circle of hues on our 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607

// This version uses precomputed image data stored in PROGMEM
// rather than calculating each pixel.  Nearly instantaneous!  Woo!

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "image.h"

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11
#define LAT 10
#define OE  9

#define A   A0
#define B   A1
#define C   A2
#define D   A3 // Comment this line out if you're using a 32x16

////////////////////////////////////////////////
/////   Instantiate RGBmatrixPanel Class   /////
////////////////////////////////////////////////

/* - One of these should be commented out!
   - Also, make sure to adjust the saved image in the <bitmap.h> file.*/

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64); // 32x64

void setup() {
  int     i, len;
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data

  // Copy image from PROGMEM to matrix buffer:
  memcpy_P(ptr, img, sizeof(img));

  // Start up matrix AFTER data is copied.  The RGBmatrixPanel
  // interrupt code ties up about 40% of the CPU time, so starting
  // it now allows the prior drawing code to run even faster!
  matrix.begin();
}

void loop() {
  // do nothing
}
