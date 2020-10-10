#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs

#include "toonLink.h"
#include "linkLandscape.h"
#include "triforce3D.h"
#include "majoras.h"
#include "link1.h"
#include "link2.h"

#define NUM_LEDS 1156

const int ledPin[] = {4, 5, 6, 7};
const int dataPin = 2;
const int changeProgramButton = 13;
const int addBrightnessButton = 11;
const int removeBrightnessButton = 12;
const int nPrograms = 6;

int brightness = 20;
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
      showAnimation(toonLink, toonLinkFrames, 50);
      break;
    case 2:
      showAnimation(linkLandscape, linkLandscapeFrames, 50);
      break;
    case 3:
      showAnimation(triforce3D, triforce3DFrames, 50);
      showAnimation(triforce3D2, triforce3DFrames, 50);
      break;
    case 4:
      showAnimation(majoras, majorasFrames, 50);
      showAnimation(majoras2, majoras2Frames, 50);
      break;
    case 5:
      showAnimation(link1, link1Frames, 50);
      break;
    case 6:
      showAnimation(link2, link2Frames, 50);
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
