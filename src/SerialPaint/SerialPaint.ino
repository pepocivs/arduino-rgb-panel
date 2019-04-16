//////////////////////
// Library Includes //
//////////////////////
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "bitmap.h"

/////////////////////
// Hardware Hookup //
/////////////////////
// R0, G0, B0, R1, G1, B1 should be connected to pins
// 2, 3, 4, 5, 6, and 7 respectively. Their pins aren't defined,
// because they're controlled directly in the library. These pins
// can be moved (somewhat):

// CLK be on PORTB!
// CLK can be moved but must remain on PORTB(8, 9, 10, 11, 12, 13)
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

/* ========== For 32x64 LED panels: ==========
  You MUST use an Arduino Mega2560 with 32x64 size RGB Panel */
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64); // 32x64

/* ========== For 32x32 LED panels: ========== */
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32

/* ==========  For 32x16 LED panels: ========== */
//RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false); // 32x16

void setup()
{
  matrix.begin();
  loadBitmap();
}

void loop()
{
  
}

void loadBitmap()
{
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data
  memcpy_P(ptr, bmp, sizeof(bmp));
}
