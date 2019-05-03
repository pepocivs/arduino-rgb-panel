#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "bulbasur.h"
#include "emojis.h"
#include "froslass.h"
#include "gaysper.h"
#include "haunter.h"
#include "rothko.h"
#include "triforce3D.h"

#define NUM_LEDS 525
#define DATA_PIN 5
#define SPEED 2000
CRGB leds[NUM_LEDS];


void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(80);
}

void loop() {
  showAnimation(haunter);
}

void showAnimation(long animation[][525]) {
  for(int x = 0; x < (sizeof(animation)/sizeof(animation[0])); x++) {
    delay(SPEED);
    FastLED.clear();
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = pgm_read_dword(&(animation[x][i]));
    }
    FastLED.show();
  }
}
