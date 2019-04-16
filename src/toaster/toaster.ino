/*
   Toaster32x32.ino -
   Flying Toasters in 32x32 RGB LED matrix
   scruss - 2017-07 - for elmwood.to
   freely modified from
   - Phillip Burgess's Animated Flying Toasters for Adafruit
    https://learn.adafruit.com/animated-flying-toaster-oled-jewelry

   - Adafruit RGB matrix examples by Limor Fried and Phillip Burgess
    https://learn.adafruit.com/32x16-32x32-rgb-led-matrix

   The toaster bitmaps are most likely lifted from Berkeley Systems'
   "After Dark" screen saver for the Apple Macintosh.
*/

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "bitmaps.h"        // animation frame definitions

// Colour definitions - format is RGB-565 16-bit
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

// array of visible colours, stepped through in loop
uint16_t colarray[] = { BLUE, RED, GREEN, CYAN,
                        MAGENTA, YELLOW, WHITE
                      };
#define COLMOD 7 // array modulus for looping
int colsub = 0; // current colour array position

// configuration for
// 32x32 matrix with SINGLE HEADER input pinout
// talking to Nootropic Matrix Backpack v2
#define CLK 11
#define LAT 10
#define OE  9

#define A   A0
#define B   A1
#define C   A2
#define D   A3 // Comment this line out if you're using a 32x16
// If your matrix has the DOUBLE HEADER input,
//  please use values from RGB matrix panel code examples
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

int i = 0; // animation frame counter

void setup() {
  matrix.begin();           // initialise LED matrix
  matrix.fillScreen(BLACK); // clear the screen
}

void loop() {
  matrix.drawBitmap(0, 0, (const uint8_t *)pgm_read_word(&mask[i]), 32, 32, BLACK);
  matrix.drawBitmap(0, 0, (const uint8_t *)pgm_read_word(&img[i]), 32, 32, colarray[colsub]);
  i++;
  if (i == 4) {
    i = 0;
    colsub++;
    colsub %= COLMOD;
  }
  delay(75);  // leisurely flying toasters ensue
}
