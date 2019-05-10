#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h" // Fastled library to control the LEDs
#include "allBlack.h"
#include "allWhite.h"
#include "redBlack.h"
#include "redWhite.h"

#define NUM_LEDS 525

const int ledPin[] = {7, 8, 9, 10};
const int dataPin = 6;
const int changeProgramButton = 5;
const int addBrightnessButton = 4;
const int removeBrightnessButton = 3;


int brightness = 10;
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
      showAnimation(allBlack, allBlackFrames, 2000);
      break;
    case 2:
      showAnimation(redBlack, redBlackFrames, 3000);
      break;
    case 3:
      showAnimation(allWhite, allWhiteFrames, 3000);
      break;
    case 4:
      showAnimation(redWhite, redWhiteFrames, 3000);
      break;
    default:
      FastLED.clear();
      break;
  }
  /*
      case 6:
      showAnimation(rothko, rothkoFrames, 1000);
      break;
    case 7:
      showAnimation(haunter, haunterFrames, 280);
      break;
    case 8:
      showAnimation(gaysper, gaysperFrames, 280);
      break;
  */
}

void showAnimation(const long animation[][525], int nFrames, int frameSpeed) {
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
    if (programNo == 15) {
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
