#include <Ticker.h>
#include <PxMatrix.h>

Ticker display_ticker;

// Pins for LED MATRIX
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_OE 2
#define P_D 12
#define P_E 0

int width = 128;
int height = 32;

PxMATRIX display(width,height,P_LAT, P_OE,P_A,P_B,P_C,P_D);

uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myORANGE = display.color565(255, 120, 0);


#include "kindomHearts.h"
#include "cromatic.h"
#include "guinness.h"

void display_updater()
{
  display.display(70);
}
void setup() {
  Serial.begin(9600);
  display.begin(16);
  display.clearDisplay();

  Serial.print("Pixel draw latency in us: ");
  unsigned long start_timer = micros();
  display.drawPixel(1, 1, 0);
  unsigned long delta_timer = micros() - start_timer;
  Serial.println(delta_timer);

  Serial.print("Display update latency in us: ");
  start_timer = micros();
  display.display(0);
  delta_timer = micros() - start_timer;
  Serial.println(delta_timer);


  display_ticker.attach(0.002, display_updater);
  yield();

  
  display.clearDisplay();
  showAnimation(guinness0);
  //display.drawPixel(0, 0, myRED);
  //display.drawPixel(0, 16, myGREEN);
  //display.drawPixel(0, 32, myBLUE);

  //display.drawPixel(16, 0, myRED);
  //display.drawPixel(16, 16, myGREEN);
  //display.drawPixel(16, 32, myBLUE);
  //display.drawPixel(16, 63, myWHITE);
  
  //display.drawPixel(16, 0, myYELLOW);
  //display.drawPixel(31, 0, myCYAN);
  //display.drawPixel(31, 0, myCYAN);
  //display.drawPixel(63, 0, myMAGENTA);
  //display.drawPixel(31, 31, myORANGE);

  //display.drawPixel(70, 70, myORANGE);

}

void loop() {
  
}

void showAnimation(const long animation[4096]) {
  int pixelCount = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint32_t colorHex = pgm_read_dword(&animation[pixelCount]);
      uint8_t r = (uint8_t)(colorHex >> 16);
      uint8_t g = (uint8_t)(colorHex >> 8);
      uint8_t b = (uint8_t)(colorHex >> 0);
      display.drawPixel(x, y, display.color565(r, g, b));
      pixelCount ++;
    }
  }
}
