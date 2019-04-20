#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

int height = 32;
int width = 64;
int nBars = 3;
int oldValues[3];
int barWidth = width / nBars;
int cadenceMax = 3;


void setup() {
  matrix.begin();
  
  for(int count = 0; count < nBars; ++count) {
    int randomNumber = rand() % height;
    oldValues[count] = height-randomNumber;
  }
}

void loop() {
 
  matrix.fillScreen(matrix.Color333(0, 0, 0)); // Put all leds black
  for(int count = 0; count < nBars; ++count) {
    int randomNumber = rand() % height;
    int barHeight = height-randomNumber;
    matrix.fillRect(count*barWidth, oldValues[count], barWidth, height, matrix.Color333(0, 1, 1));
    matrix.drawRect(count*barWidth, oldValues[count], barWidth, height, matrix.Color333(0, 5, 5));
    if (randomNumber%2 == 0) {
      oldValues[count] = oldValues[count] + rand()%cadenceMax;
    } else {
      oldValues[count] = oldValues[count] - rand()%cadenceMax;
    }
    if (oldValues[count] < 0) oldValues[count] = 0;
    if (oldValues[count] > height) oldValues[count] = height;
  }
  delay(100);
}
