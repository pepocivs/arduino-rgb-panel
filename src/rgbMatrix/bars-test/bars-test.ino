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
bool decrease[3];
int barWidth = width / nBars;
int cadenceMax = 12;
int border = 2;
int spaceBetween = (border*2);


void setup() {
  matrix.begin();
  
  for(int count = 0; count < nBars; ++count) {
    int randomNumber = rand() % height;
    oldValues[count] = height-randomNumber;
  }
}

void loop() {
  for(int count = 0; count < nBars; ++count) {
    int startX = (count*barWidth) + border;
    int startY = (oldValues[count]) + border;
    
    if (decrease[count] == true) { // Clean screen just at the points needed
      matrix.fillRect(startX, 0, barWidth, oldValues[count], matrix.Color333(0, 0, 0));
    }
    // Fill the rectangle
    matrix.fillRect(startX, startY, barWidth-spaceBetween, height, matrix.Color888(0x1E, 0xAF, 0x95));
    
    if ((rand()%height)%2 == 0) {
      oldValues[count] = oldValues[count] + rand()%cadenceMax;
      decrease[count] = true;
    } else {
      oldValues[count] = oldValues[count] - rand()%cadenceMax;
      decrease[count] = false;
    }
    
    if (oldValues[count] < cadenceMax) oldValues[count] = cadenceMax;
    if (oldValues[count] > height) oldValues[count] = height;
  }
}
