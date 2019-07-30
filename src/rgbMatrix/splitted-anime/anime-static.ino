#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#include "kindomHearts.h"
#include "link.h"
#include "finalFantasy.h"
#include "pokemon.h"
#include "pokemonPlata.h"
#include "pokemonGhosts.h"
#include "pokemonGhosts2.h"
#include "pokemonGhosts3.h"
#include "pokemonGhosts4.h"
#include "casaDePapel.h"
#include "casaDePapel2.h"
#include "guinness.h"

#define OE   9
#define LAT 10
#define CLK 11
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  matrix.begin();
}

void loop() {
  showAnimation(kindomHearts, kindomHeartsFrames, kindomHeartsWidth, kindomHeartsHeight, 10);
  delay(20000);
  showAnimation(link, linkFrames, linkWidth, linkHeight, 10);
  delay(20000);
  showAnimation(pokemon, pokemonFrames, pokemonWidth, pokemonHeight, 10);
  // delay(20000);
  // showAnimation(pokemonPlata, pokemonPlataFrames, pokemonPlataWidth, pokemonPlataHeight, 10);
  delay(20000);
  showAnimation(pokemonGhosts, pokemonGhostsFrames, pokemonGhostsWidth, pokemonGhostsHeight, 10);
  delay(20000);
  showAnimation(pokemonGhosts2, pokemonGhosts2Frames, pokemonGhosts2Width, pokemonGhosts2Height, 10);
  delay(20000);
  showAnimation(pokemonGhosts3, pokemonGhosts3Frames, pokemonGhosts3Width, pokemonGhosts3Height, 10);
  delay(20000);
  showAnimation(pokemonGhosts4, pokemonGhosts4Frames, pokemonGhosts4Width, pokemonGhosts4Height, 10);
  delay(20000);
  showAnimation(casaDePapel, casaDePapelFrames, casaDePapelWidth, casaDePapelHeight, 10);
  delay(20000);
  showAnimation(casaDePapel2, casaDePapel2Frames, casaDePapel2Width, casaDePapel2Height, 10);
  delay(20000);
  showAnimation(guinness, guinnessFrames, guinnessWidth, guinnessHeight, 10);
  delay(20000);
}


void showAnimation(const uint8_t animation[][2048][3], int nFrames, int width, int height, int frameSpeed) {
  for(int frame = 0; frame < nFrames; frame++) {
    delay(frameSpeed);
    int pixelCount = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        uint8_t r = pgm_read_byte(&(animation[frame][pixelCount][0]));
        uint8_t g = pgm_read_byte(&(animation[frame][pixelCount][1]));
        uint8_t b = pgm_read_byte(&(animation[frame][pixelCount][2]));
        matrix.drawPixel(x, y, matrix.Color888(r, g, b, true));
        pixelCount ++;
      }
    }
  }
}
