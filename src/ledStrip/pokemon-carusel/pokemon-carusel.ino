#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "haunter.h"


#define NUM_LEDS 525
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

const short tiempo = 1000;


 void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(40);
}

void loop() {
  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(haunter[i])); }
  FastLED.show();

}
