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
  int ejeX = posicionXInicial + (anchuraBarra * nBarra) + nBarra;
  int ejeY = alturaMaxBarra - altura;
  int auxY = ejeY;
  int maxAlturaSeccion = 6;
  int alturaSeccion = 0;
  int colorArray[5][3] = {  
   {5, 0, 0},
   {0, 5, 0},
   {0, 0, 5},
   {0, 5, 5},
   {5, 5, 0}
};
  
  matrix.fillRect(ejeX, 0, anchuraBarra, ejeY, matrix.Color333(0, 0, 0));
  // matrix.fillRect(ejeX, ejeY, anchuraBarra, altura, matrix.Color333(0, 5, 5));

  int nSecciones = (int)floor(altura/5);
  int resto = altura%5;
  int lastAltura = (altura > 30) ? 8 : 6;
  
  for (int x = 0; x<nSecciones; ++x) {
    auxY = alturaMaxBarra - ((x+1) * maxAlturaSeccion);
    alturaSeccion = maxAlturaSeccion;
    if (nSecciones == (x+1)) {
      alturaSeccion = resto;
      auxY = ejeY;
    }
    matrix.fillRect(ejeX, auxY, anchuraBarra, alturaSeccion, matrix.Color333(colorArray[x][0],colorArray[x][1],colorArray[x][2]));
  }
  /*
  if (ejeY < 8) { 
    matrix.fillRect(ejeX, ejeY, anchuraBarra, 8 - ejeY, matrix.Color888(0xaa,0x00,0x00));
  }
  if (ejeY < 14) { 
    matrix.fillRect(ejeX, 8, anchuraBarra, 6, matrix.Color888(0x00,0xaa,0x00));
  }
  if (ejeY < 20) { 
    matrix.fillRect(ejeX, 14, anchuraBarra, 6, matrix.Color888(0x00,0x00,0xaa));
  }
  if (auxY < 26) { 
    matrix.fillRect(ejeX, 20, anchuraBarra, 6, matrix.Color888(0x00,0xaa,0xaa));
  }
  if (auxY < 32) { 
   matrix.fillRect(ejeX, 26, anchuraBarra, 6, matrix.Color888(0xaa,0xaa,0xaa));
  }
  */
}


void loop() {
 
  pintarBarra(0,28);
  pintarBarra(1,31);
  pintarBarra(2,2);
    
  delay(tiempoEspera);

 
}
