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
  matrix.fillRect(ejeX, 0, anchuraBarra, ejeY, matrix.Color333(0, 0, 0));
  // matrix.fillRect(ejeX, ejeY, anchuraBarra, altura, matrix.Color333(0, 5, 5)); 
  if (ejeY < 8) { 
    matrix.fillRect(ejeX, ejeY, anchuraBarra, 8 - ejeY, matrix.Color888(0x00,0x37,0x58));
    auxY = ejeY + 8 - ejeY;
  }
  if (ejeY < 14) { 
    matrix.fillRect(ejeX, auxY, anchuraBarra, 14 - ejeY, matrix.Color888(0x00,0xa8,0x84));
    auxY = auxY + 6;
  }
  if (ejeY < 20) { 
    matrix.fillRect(ejeX, ejeY + auxY, anchuraBarra, auxY, matrix.Color888(0x00,0xfc,0xdc));
    auxY = auxY - 6;
  }
  /*
   if (auxY < 32) { 
    matrix.fillRect(ejeX, auxY, anchuraBarra, 8 - ejeY, matrix.Color888(0xff,0xff,0xff));
    auxY = 24;
   }
   if (auxY < 26) { 
    matrix.fillRect(ejeX, auxY, anchuraBarra, 8 - ejeY, matrix.Color888(0x40,0xff,0xfe));
    auxY = 18;
   }
   if (auxY < 20) { 
    matrix.fillRect(ejeX, auxY, anchuraBarra, 8 - ejeY, matrix.Color888(0x00,0xfc,0xdc));
    auxY = 12;
   }
  
    */ 
}


void loop() {
 
  pintarBarra(0,29);
  pintarBarra(1,16);
  pintarBarra(2,8);
    
  delay(tiempoEspera);

 
}
