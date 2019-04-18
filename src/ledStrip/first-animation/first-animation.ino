#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "step01.h"
#include "step02.h"

#define NUM_LEDS 225
#define DATA_PIN 3
CRGB leds[NUM_LEDS];


 void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(15);
}

void loop() {
  delay(500);
  
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(step01[i]));
  }
  
  FastLED.show();
  delay(500);

  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(step02[i]));
  }
  
  FastLED.show();
}
