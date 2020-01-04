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

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=75; //10-50 is usually fine

PxMATRIX display(width,height,P_LAT, P_OE,P_A,P_B,P_C,P_D);

#include "navidad.h"
#include "fireplace.h"

void display_updater()
{
  display.display(display_draw_time);
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
  display.flushDisplay();
  display_ticker.attach(0.002, display_updater);
  yield();
  display.clearDisplay();

}

void loop() {
  showAnimation(fireplace, fireplaceFrames, 800, 30000);
  repeatAnimation(navidad, navidadFrames, 800, 10);
}

void repeatAnimation(const long animation[][4096], int nFrames, int delayTime, int times) {
  for (int sequence = 0; sequence < times; sequence++) {
    showAnimation(animation, nFrames, delayTime, 0);
  }
}

void showAnimation(const long animation[][4096], int nFrames, int delayTime, int afterTime) {
  for (int frame = 0; frame < nFrames; frame++) {
    showFrame(animation[frame]);
    delay(delayTime);
  }
  delay(afterTime);
}

void showFrame(const long animation[4096]) {
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
