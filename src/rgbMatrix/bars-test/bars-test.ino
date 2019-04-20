#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

int width = 32;
int randomNumber = rand() % 20;

void setup() {

  matrix.begin();
  
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.drawRect(0, width-20, 10, width, matrix.Color333(0, 7, 7));

  // whew!
}

void loop() {

}
