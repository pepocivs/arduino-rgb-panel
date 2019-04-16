/* Serial Paint (RGB Panel Paint)
   by: Jim Lindblom @ SparkFun Electronics
   date: December 9, 2013
   modified by: Ho Yun "Bobby" Chan @ SparkFun Electronics
   date: May 10, 2018

   license: Beerware. Please use, reuse, and modify this sketch.
   If you find it useful, and we meet some day, we can share a beer.

   Description: This sketch serves as a fun, interactive demo
   for the following RGB Panels.

     32x64, 1:16 scan | https://www.sparkfun.com/products/14718
     32x32, 1:16 scan | https://www.sparkfun.com/products/12584
     16x32, 1:8 scan | https://www.sparkfun.com/products/retired/12583

   After loading the sketch, and hooking everything up, you can
   control the panel from your serial monitor (we actually
   recommend using a real serial program, like Tera Term). 9600
   bps.

   Your cursor should blink the active color. Controls are:
     Movement: w, a, s, d (up, down, left, right)
     Draw Pixel: Spacebar
     Erase Pixel: e
     Erase Screen: E
     Fill screen active color: f
     Red value up: R (values between 0 and 7)
     Red value down: r
     Green value up/down: G/g
     Blue value up/ down: B/b
     Copy color: z copies a color under the cursor
     Draw line: press v to place starting point. Then move cursor
     to endpoint and press v again.
     Draw Box: press x or X to place first corner. Then move
     your cursor to where you want the diagonal corner. Then press
     either x for a no-fill box, or X for a filled box.
     Draw Circle: press c or C to place the _center_ of the circle.
     Then move your cursor to where you want the outside edge of
     your circle to be. Then press c for a no-fill circle or C for
     a filled circle.
     Text mode: press t to go to text mode. Now any characters
     received will be displayed on the panel. It'll wrap around
     from one line to the next, but not from bottom to top.
     Press ` (above Tab / left of 1) to exit text mode.
     Print: press p to print an array

   Check out our hookup guide for help wiring it all up:
     https://learn.sparkfun.com/tutorials/rgb-panel-hookup-guide

   You'll need to download and install the RGBmatrixPanel and
   Adafruit_GFX libraries:
     https://github.com/adafruit/RGB-matrix-Panel
     https://github.com/adafruit/Adafruit-GFX-Library

   This sketch uses the RGBmatrixPanel and Adafruit_GFX libraries,
   written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
   for Adafruit Industries. Released under the BSD license.
*/

//////////////////////
// Library Includes //
//////////////////////
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "bitmap.h"

/////////////////////
// Hardware Hookup //
/////////////////////
// R0, G0, B0, R1, G1, B1 should be connected to pins
// 2, 3, 4, 5, 6, and 7 respectively. Their pins aren't defined,
// because they're controlled directly in the library. These pins
// can be moved (somewhat):

// CLK be on PORTB!
// CLK can be moved but must remain on PORTB(8, 9, 10, 11, 12, 13)
#define CLK 11
#define LAT 10
#define OE  9

#define A   A0
#define B   A1
#define C   A2
#define D   A3 // Comment this line out if you're using a 32x16

////////////////////////////////////////////////
/////   Instantiate RGBmatrixPanel Class   /////
////////////////////////////////////////////////

/* - One of these should be commented out!
   - Also, make sure to adjust the saved image in the <bitmap.h> file.*/

/* ========== For 32x64 LED panels: ==========
  You MUST use an Arduino Mega2560 with 32x64 size RGB Panel */
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64); // 32x64

/* ========== For 32x32 LED panels: ========== */
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32

/* ==========  For 32x16 LED panels: ========== */
//RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false); // 32x16

//////////////////////////

///////////////////////////
// Paint Program Globals //
///////////////////////////
// Enum UP, DOWN, LEFT, RIGHT to make programming easier:
enum direction {
  UP,    // 0
  DOWN,  // 1
  LEFT,  // 2
  RIGHT  // 3
};

int8_t cursorX = 0;  // Cursor x position, initialize left
int8_t cursorY = 0;  // Cursor y position, initialize top
int16_t oldColor = 0; // Keep track of color under cursor
int8_t red = 1;   // Red paint value
int8_t blue = 1;  // Blue paint value
int8_t green = 1; // Green paint value

// Enum shape drawing mode:
enum shapeMode {
  NONE,   // 0
  LINE,   // 1
  CIRCLE, // 2
  RECT    // 3
};
int8_t activeDraw = NONE; // Keeps track of whether we're drawing a shape
int16_t oldCenterColor; // For circle, keeps track of color under cursor
int8_t shapeX0;  // x0 of line or box, center of circle
int8_t shapeY0;  // y0 of line or box, center of circle

#define EXIT_CHAR '`'
// Timing stuff:
#define BLINK_RATE 250 // blink rate in ms

// The setup() function initializes the matrix, and clears the
// screen. It also starts Serial.
void setup()
{
  matrix.begin();       // Initialize the matrix.
  blankEasel(); // Blank screen
  loadBitmap();
  Serial.begin(9600);   // Start serial
}

// loop() accomplishes two tasks:
//  1) Blink the cursor if enough time has passed.
//  2) Check if Serial data is received, and call the necessary
//     function to act on that data.
void loop()
{
  // These variables keep track of the cursor status. lastUpdate
  // remembers when we last did something to the cursor.
  // cursorStatus remembers if the cursor is on or off.
  static uint32_t lastUpdate = 0;
  static boolean cursorStatus = true;
  // If enough time has passed, let's blink the cursor:
  if (millis() > lastUpdate + BLINK_RATE)
  {
    lastUpdate = millis(); // get a new lastUpdate value
    blinkCursor(cursorStatus); // blinkCursor on or off
    cursorStatus = !cursorStatus; // Update cursorStatus
  }

  // This huge block checks if serial data is available. If so
  // it'll act on that data:
  if (Serial.available())
  {
    char c = Serial.read();     // Read data into c
    switch (c)
    {
      //////////////
      // Movement //
      //////////////
      case 'w':                 // w - Move up
        moveCursor(UP, 1);
        break;
      case 'W':                 // W - move up 2
        moveCursor(UP, 2);
        break;
      case 's':                 // s - Move down
        moveCursor(DOWN, 1);
        break;
      case 'S':                 // S - Move down 2
        moveCursor(DOWN, 2);
        break;
      case 'a':                 // a - Move left
        moveCursor(LEFT, 1);
        break;
      case 'A':                 // A - Move left 2
        moveCursor(LEFT, 2);
        break;
      case 'd':                 // d - Move Right
        moveCursor(RIGHT, 1);
        break;
      case 'D':                 // D - Move right 2
        moveCursor(RIGHT, 2);
        break;
      ///////////////////
      // Color Control //
      ///////////////////
      case 'r': // Red down
        if (red > 0) red--;
        break;
      case 'R': // Red up
        if (red < 7) red++;
        break;
      case 'b': // blue down
        if (blue > 0) blue--;
        break;
      case 'B': // blue up (hehe)
        if (blue < 7) blue++;
        break;
      case 'g': // green down
        if (green > 0) green--;
        break;
      case 'G': // green up
        if (green < 7) green++;
        break;
      case 'z': // Copy color
        copyColor();
      /////////////////////////
      // Draw / Erase Pixels //
      /////////////////////////
      case ' ':                 // Space - Place pixel
        drawDot(true);
        break;
      case 'e':                 // e - Erase pixel
        drawDot(false);
        break;
      /////////////////
      // Draw Shapes //
      /////////////////
      case 'v':                 // v - draw a vector (line)
        drawLine();
        break;
      case 'x':                 // x - start box / draw box
        drawBox(false);
        break;
      case 'X':                 // X - start box / fill box
        drawBox(true);
        break;
      case 'c':                 // c - start circle / draw circle
        drawCircle(false);
        break;
      case 'C':                 // C - start circle / fill circle
        drawCircle(true);
        break;
      case 't':                 // t - Enter text mode
        drawText();
        break;
      case 'F':
      case 'f':                 // f - Fill screen with active color
        matrix.fillScreen(matrix.Color333(red, green, blue));
        drawDot(true);
        break;
      case 'E':                 // E - Erase screen, reset cursor
        blankEasel();
        break;
      case 'p':
        printBuffer();
        break;
      case 'l':
        loadBitmap();
        break;
    }
  }
}

// Draw dot places a pixel where the cursorX and cursorY variables
// are.
// Color is determined by the draw boolean parameter. 0 erases a
// pixel (draws blank). 1 draws the active color.
void drawDot(boolean draw)
{
  // We need to both update the oldColor varaiable, which keeps
  // track of a color under the cursor. And send the drawPixel
  // command. First update oldColor:
  if (draw) // if 1 Draw pixel
    oldColor = matrix.Color333(red, green, blue);
  else      // else Erase
    oldColor = 0;
  // Then draw the desired color:
  matrix.drawPixel(cursorX, cursorY, oldColor);
}

// drawLine should be called twice to draw a line. The first
// time it's called (if activeDraw is not LINE), it'll set up the
// global variables it needs. The second time it's called, it
// will actually draw the line.
void drawLine()
{
  // activeDraw is a global variable which keeps track of which
  // (if any) shape drawing mode we're in.
  if (activeDraw != LINE) // If we're not already in LINE mode:
  {
    drawDot(true);  // Place a pixel where the cursor is
    shapeX0 = cursorX;  // Update shapeX0 variable with current x
    shapeY0 = cursorY;  // Update shapeY0 variable with current y
    activeDraw = LINE;  // Next time drawLine is called we'll draw
  }
  else // Otherwise, we're in line mode and we already have
  { // the first point
    // Draw a line from previously stored variables to the
    // current cursor position:
    matrix.drawLine(shapeX0, shapeY0, cursorX, cursorY,
                    matrix.Color333(red, green, blue));
    drawDot(true);  // Draw a dot (update oldColor variable)
    activeDraw = NONE; // Revert to no shape-mode active
  }
}

// drawBox should be called twice to draw a box. First to set the
// first corner. Second to set the diagonally opposite corner,
// and actually draw the box.
void drawBox(boolean fill)
{
  if (activeDraw != RECT) // If we're not already in RECT mode:
  {
    drawDot(true);  // Place a pixel where the cursor is
    shapeX0 = cursorX; // Set shapeX0 to current x position
    shapeY0 = cursorY; // Set shape Y0 to current y position
    activeDraw = RECT; // Next time this is called, we'll draw
  }
  else // Otherwise, we can draw a rectangle:
  {
    // Need to calculate the width and height of the box, based
    // on previously stored values and current cursor position:
    int16_t w = abs(cursorX - shapeX0) + 1;
    int16_t h = abs(cursorY - shapeY0) + 1;
    // drawRect draws from top-left corner, find that by getting
    // the minimum coordinate of each corner:
    shapeX0 = min(shapeX0, cursorX);
    shapeY0 = min(shapeY0, cursorY);
    // Use all the values we calculated to draw a box:
    if (fill)
      matrix.fillRect(shapeX0, shapeY0, w, h,
                      matrix.Color333(red, green, blue));
    else
      matrix.drawRect(shapeX0, shapeY0, w, h,
                      matrix.Color333(red, green, blue));
    drawDot(true);  // Draw a pixel (just  to update oldColor)
    activeDraw = NONE; // Back to no active shape mode
  }
}

// drawCircle should be called twice to draw a circle. Call first
// to set the position of the center. Call again to set the
// outer radius value.
void drawCircle(boolean fill)
{
  if (activeDraw != CIRCLE) // If we're not already in CIRCLE mode
  {
    // Let's place a pixel so we can visually see the center.
    // We may need to remove the pixel (if not filling) so store
    // the old pixel color first.
    oldCenterColor = oldColor;
    drawDot(true);  // Place pixel where cursor is
    shapeX0 = cursorX; // Store x and y so we remember where
    shapeY0 = cursorY; // the center is
    activeDraw = CIRCLE; // Next time we'll draw a circle:
  }
  else
  {
    // We'll need to calculate the radius of the circle. Time
    // to whip out some pythagorean theorum!
    // First, find the x and y distances from center:
    uint8_t x = abs(cursorX - shapeX0);
    uint8_t y = abs(cursorY - shapeY0);
    // Then r^2 = x^2 + y^2, solve for r
    int16_t radius = (int16_t) sqrt(x * x + y * y);
    // Now we can draw our circle:
    if (fill)
    {
      // Draw a filled circle:
      matrix.fillCircle(shapeX0, shapeY0, radius,
                        matrix.Color333(red, green, blue));
    }
    else // Empty circle
    {
      // Draw an empty circle:
      matrix.drawCircle(shapeX0, shapeY0, radius,
                        matrix.Color333(red, green, blue));
      // Draw the old color back in the center:
      matrix.drawPixel(shapeX0, shapeY0, oldCenterColor);
    }
    drawDot(true); // Draw pixel at cursor, update oldColor
    activeDraw = NONE; // Revert back to no active shape mode
  }
}

// drawText is used to draw text on the display. It'll enter a
// loop, continuously receiving serial chars and printing them.
// It'll exit when EXIT_CHAR is received over serial.
void drawText()
{
  char c;
  blinkCursor(0); // Turn cursor off

  // Setup text color and cursor:
  matrix.setTextColor(matrix.Color333(red, green, blue));
  matrix.setCursor(cursorX, cursorY);

  // Print characters until receive EXIT_CHAR (`)
  while (c != EXIT_CHAR)
  {
    while (!Serial.available()) ; // loop and wait for serial
    c = Serial.read();  // read serial data into c
    if (c != EXIT_CHAR)
    {
      if (c == '\n')   // CTRL+ENTER = newline / linefeed
      {
        cursorY += 8; // Increase another line
        cursorX = 0;  // Back to beginning
        matrix.setCursor(cursorX, cursorY); // Update cursor
      }
      else if (c == '\r') // ENTER
      {
        cursorY += 8; // Increase another line
        matrix.setCursor(cursorX, cursorY); // Update cursor
      }
      else // Otherwise it's a printable character
      {
        matrix.print(c); // Print char
        cursorX += 6; // Update our cursor
        if (cursorX >= (matrix.width() - 6))
        { // If x is to far to the right, go down a line:
          cursorX = 0;
          cursorY += 8;
        }
      }
    }
  }
  // Check to make sure y doesn't go off screen:
  if (cursorY >= matrix.height())
    cursorY = matrix.height() - 1;
}

// moveCursor moves the cursorX and cursorY variables UP, DOWN,
// LEFT, or RIGHT in distance number of spaces. It'll wrap around
// the screen if moved out of bounds.
// This function also keeps track of any old pixels under the
// cursor. It'll redraw them when the cursor moves.
void moveCursor(uint8_t direction, uint8_t distance)
{
  // If there was a color under the cursor, put it back:
  matrix.drawPixel(cursorX, cursorY, oldColor);

  // For each direction, decrease it by distance, then check
  // to make sure it's not out of bounds. If it is, wrap around
  // to the other side.
  switch (direction)
  {
    case UP:
      cursorY -= distance;
      if (cursorY < 0) cursorY = matrix.height() - 1;
      break;
    case DOWN:
      cursorY += distance;
      if (cursorY > matrix.height() - 1) cursorY = 0;
      break;
    case LEFT:
      cursorX -= distance;
      if (cursorX < 0) cursorX = matrix.width() - 1;
      break;
    case RIGHT:
      cursorX += distance;
      if (cursorX > matrix.width() - 1) cursorX = 0;
      break;
  }
  // update oldColor before the loop() redraws our cursor.
  oldColor = getPixelColor(cursorX, cursorY);
}

// blinkCursor turns the cursor either on or off. Cursor color
// is determined by red, green, and blue variable values.
void blinkCursor(boolean stat)
{
  if (stat) // Blink on:
    matrix.drawPixel(cursorX, cursorY, matrix.Color333(red, green, blue));
  else      // Blink off
    matrix.drawPixel(cursorX, cursorY, 0);
}

// This is a slightly modified version of the dumpMatrix function
// from the RGBmatrixPanel library. Running out of SRAM! So some
// of the static serial prints are stored in flash ("F()"). Also
// do some extra formatting so it works with non-Serial monitor
// terminals.
void printBuffer()
{
  blinkCursor(false); // Turn cursor off first.

  int bufferSize = matrix.width() * (matrix.height() / 2) * 3;
  // Get a pointer to the data buffer:
  uint8_t * buf = matrix.backBuffer();

  // Print some stuff
  Serial.println();
  Serial.print(F("#include <avr/pgmspace.h>\r\n\r\n"));
  Serial.print(F("static const uint8_t PROGMEM bmp[] = {\r\n"));

  for (int i = 0; i < bufferSize; i++)
  {
    Serial.print(F("0x"));
    if (buf[i] < 0x10)
      Serial.print('0');
    Serial.print(buf[i], HEX);
    if (i < (bufferSize - 1))
    {
      if ((i & 7) == 7)
        Serial.print(F(", \r\n "));
      else
        Serial.print(',');
    }
  }
  Serial.println(F("\r\n};"));
}

// Copy color updates the red, green, and blue variables to match
// the oldColor variable values.
void copyColor()
{
  red = (oldColor & 0xE000) >> 13;
  green = (oldColor & 0x0700) >> 8;
  blue = (oldColor & 0x001C) >> 2;
}

// loadBitmap loads up a stored bitmap into the matrix's data
// buffer. If you've printed an image out, use this to load it
// back onto the screen.
void loadBitmap()
{
  uint8_t *ptr = matrix.backBuffer(); // Get address of matrix data
  memcpy_P(ptr, bmp, sizeof(bmp));

  // update oldColor before the loop() redraws our cursor.
  oldColor = getPixelColor(cursorX, cursorY);
}

// Reset the screen. Set cursors back to top-left. Reset oldColor
// and blank the screen.
void blankEasel()
{
  cursorX = 0;
  cursorY = 0;
  oldColor = 0;
  matrix.fillScreen(0); // Blank screen
}

// getPixelColor digs into the rgbMatrixPanel library, and the
// buffer where it stores data, to retreive the color of a pixel.
// Most of this was convoluted from the drawPixel function in the
// rgbMatrixPanel library.
// This function is called in the moveCursor function, to restore
// a pixel color if the cursor is on top of it.
uint16_t getPixelColor(uint8_t x, uint8_t y)
{
  uint8_t * ptr; // A movable pointer to read data from
  uint16_t r = 0, g = 0, b = 0; // We'll store rgb values in these
  uint8_t nPlanes = 4; // 4 planes. This is static in the library
  uint8_t nRows = matrix.height() / 2; // 16 for 32 row, 8 for 16

  // Get a pointer to the matrix backBuffer, where the pixel/
  // color data is stored:
  uint8_t * buf = matrix.backBuffer();

  // Data for upper half is stored in lower bits of each byte
  if (y < nRows)
  {
    ptr = &buf[y * matrix.width() * (nPlanes - 1) + x];
    // get data from plane 0:
    if (ptr[64] & 0x01)
      r = 1;
    if (ptr[64] & 0x02)
      g = 1;
    if (ptr[32] & 0x01)
      b = 1;
    // get data from other planes:
    for (uint8_t i = 2; i < (1 << nPlanes);  i <<= 1)
    {
      if (*ptr & 0x04)
        r |= i;
      if (*ptr & 0x08)
        g |= i;
      if (*ptr & 0x10)
        b |= i;
      ptr += matrix.width();
    }
  }
  else
  {
    ptr = &buf[(y - nRows) * matrix.width() * (nPlanes - 1) + x];
    // get data from plane 0
    if (ptr[32] & 0x02)
      r = 1;
    if (*ptr & 0x01)
      g = 1;
    if (*ptr & 0x02)
      b = 1;
    for (uint8_t i = 2; i < (1 << nPlanes); i <<= 1)
    {
      if (*ptr & 0x20)
        r |= i;
      if (*ptr & 0x40)
        g |= i;
      if (*ptr & 0x80)
        b |= i;
      ptr += matrix.width();
    }
  }
  return (r << 12) | (g << 7) | (b << 1);
}
