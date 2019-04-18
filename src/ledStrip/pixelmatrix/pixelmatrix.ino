#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "heart.h"
#include "weedle.h"
#include "rgb.h"
#include "clock.h"
#include "hello.h"

#define NUM_LEDS 225
#define DATA_PIN 3
CRGB leds[NUM_LEDS];


 void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(15);
}

void loop() {
  delay(2000);
  
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(hello[i]));
  }
  
  FastLED.show();
  delay(2000);

  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(heart[i]));
  }
  
  FastLED.show();
  delay(2000);

  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(rgb[i]));
  }
  
  FastLED.show();
  delay(2000);

  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(reloj[i]));
  }
  FastLED.show();

  delay(2000);

  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword(&(weedle[i]));
  }
  FastLED.show();


}
