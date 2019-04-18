#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "heart.h"
#include "weedle.h"
#include "rgb.h"
#include "clock.h"

#define NUM_LEDS 225
#define DATA_PIN 3
CRGB leds[NUM_LEDS];


 void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(15);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = pgm_read_dword(&(reloj[i]));
    }
  FastLED.show();
}

void loop() {

}
