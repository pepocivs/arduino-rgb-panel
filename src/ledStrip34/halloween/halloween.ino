#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs

#include "halloween01.h"
#include "halloween02.h"
#include "halloween03.h"

#define NUM_LEDS 1156

const int ledPin[] = {4, 5, 6, 7};
const int dataPin = 2;
const int changeProgramButton = 13;
const int addBrightnessButton = 11;
const int removeBrightnessButton = 12;
const int nPrograms = 3;

int brightness = 40;
int programButtonState = 0;
int brightnessButtonState = 0;
int programNo = 1;

CRGB leds[NUM_LEDS];

void setup() {
  for (int i =0;i<4;i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(changeProgramButton, INPUT_PULLUP);
  pinMode(addBrightnessButton, INPUT_PULLUP);
  pinMode(removeBrightnessButton, INPUT_PULLUP);
  displayBinary(programNo);
  FastLED.addLeds<NEOPIXEL,dataPin>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
}

void loop() {
  changeProgram(changeProgramButton);
  changeBrightness(addBrightnessButton, 1);
  changeBrightness(removeBrightnessButton, -1);
  switch (programNo) {
    case 1:
      showAnimation(halloween01, halloween01Frames, 250);
      break;
    case 2:
      showAnimation(halloween02, halloween02Frames, 250);
      break;
    case 3:
      showAnimation(halloween03, halloween03Frames, 250);
      break;
    default:
      FastLED.clear();
      break;
  }
}

void showAnimation(const long animation[][1156], int nFrames, int frameSpeed) {
  for(int x = 0; x < nFrames; x++) {
    delay(frameSpeed);
    FastLED.clear();
    for(int i = 0; i < NUM_LEDS; i++) {
      changeBrightness(addBrightnessButton, 1);
      changeBrightness(removeBrightnessButton, -1);
      leds[i] = pgm_read_dword(&(animation[x][i]));
    }
    FastLED.show();
  }
}

void changeProgram(int programButton) {
  programButtonState = digitalRead(programButton);
  if (programButtonState == LOW) {
    delay(300);
    if (programNo == nPrograms) {
      programNo = 1;
      displayBinary(programNo);
    } else {
      programNo ++;
      displayBinary(programNo);
    }
  }
}

void changeBrightness(int brightnessButton, int addRemove) {
  brightnessButtonState = digitalRead(brightnessButton);
  if (brightnessButtonState == LOW) {
    delay(300);
    if (brightness > 100) {
      brightness = 100;
      FastLED.setBrightness(brightness);
    } else if (brightness < 5) {
      brightness = 5;
      FastLED.setBrightness(brightness);
    } else { 
      brightness = brightness + (5 * addRemove);
      FastLED.setBrightness(brightness);
    }
  }
}

void displayBinary(byte numToShow) {
  for (int i =0;i<4;i++) {
    if (bitRead(numToShow, i)==1) {
      digitalWrite(ledPin[i], HIGH); 
    } else {
      digitalWrite(ledPin[i], LOW); 
    }
  }
}
