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

int alturaMaxBarra = 32;
int posicionXInicial = 12;
int anchuraBarra = 12;
int tiempoEspera = 125;
void setup() {
  matrix.begin();
}

void pintarBarra(int nBarra, int altura) {
  matrix.fillRect(posicionXInicial + (anchuraBarra * nBarra) + nBarra, alturaMaxBarra - altura, anchuraBarra, alturaMaxBarra, matrix.Color333(0, 7, 7));  
}

void loop() {
  matrix.fillScreen(matrix.Color333(3, 0, 0));
  pintarBarra(0,20);
  pintarBarra(1,16);
  pintarBarra(2,8);
    
  delay(tiempoEspera);
  matrix.fillScreen(matrix.Color333(3, 0, 0));
  pintarBarra(0,18);
  pintarBarra(1,20);
  pintarBarra(2,15);
  delay(tiempoEspera);
}
