#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "step01.h"
#include "step02.h"
#include "step03.h"
#include "step04.h"
#include "step05.h"
#include "step06.h"
#include "step07.h"
#include "step08.h"
#include "step09.h"
#include "step10.h"
#include "step11.h"
#include "step12.h"


#define NUM_LEDS 225
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

const short tiempo = 50;


 void setup() { 
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);  // Init of the Fastled library
  FastLED.setBrightness(50);
}

void loop() {
  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step01[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step02[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step03[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step04[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step05[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step06[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step07[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step08[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step09[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step10[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step11[i])); }
  FastLED.show();

  delay(tiempo);
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) { leds[i] = pgm_read_dword(&(step12[i])); }
  FastLED.show();
}
