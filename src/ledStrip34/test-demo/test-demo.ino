#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "test.h"

#define NUM_LEDS 1156

const int dataPin = 2;
int brightness = 40;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL,dataPin>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
}

void loop() {
  showAnimation(test, testFrames, 3000);
}

void showAnimation(const long animation[][1156], int nFrames, int frameSpeed) {
  for(int x = 0; x < nFrames; x++) {
    delay(frameSpeed);
    FastLED.clear();
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = pgm_read_dword(&(animation[x][i]));
    }
    FastLED.show();
  }
}
