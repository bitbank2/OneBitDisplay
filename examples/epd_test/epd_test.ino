//
// OneBitDisplay EPD (e-ink) demo sketch
// Written by Larry Bank 7/10/2022
//
// This program shows how to use epd panels
// with the OneBitDisplay library. The general idea
// is that it doesn't support "immediate" mode with e-ink. 
// You'll need to allocate a buffer to hold the pixel
// data, draw on it, then dump it all at once to the panel
// as a full update (display) or partial (displayPartial).
// This demo draws some text using the built-in fonts and
// a custom font in Adafruit_GFX format
//
// The target hardware for this demo is the Pimoroni Badger2040
// A RPI Pico MCU with a UC8151 2.9" 128x296 1-bpp epd
//
#include <OneBitDisplay.h>
#include "Roboto_Black_40.h"

ONE_BIT_DISPLAY lcd;

#define BADGER_FREQ 12000000
#define BADGER_CS 17
#define BADGER_BUSY -1
#define BADGER_RES 21
#define BADGER_DC 20
#define BADGER_MOSI 19
#define BADGER_SCK 18
#define BADGER_BATT 29

void setup() {
  // Initialize the library by telling it how the display is connected
  lcd.setSPIPins(BADGER_CS, -1, -1, BADGER_DC, BADGER_RES, BADGER_BUSY);
  lcd.SPIbegin(EPD29_128x296, BADGER_FREQ); // 12Mhz is fast enough

  // Set the orientation to draw in the direction we want
  lcd.setRotation(90); // the panel is actually 128 wide by 296 tall, but oriented 90 right
  lcd.allocBuffer(); // allocate a back buffer and keep it internal to the class
  lcd.fillScreen(OBD_WHITE); // 0 for EPDs is white (for OneBitDisplay)
  lcd.setFont(FONT_12x16); // use the internal stretched/smoothed font
  lcd.print("This was a full update");
  lcd.display(); // do a full update
  for (int i=24; i<120; i += 32) {
    lcd.fillCircle(i, i, 8, 1);
    lcd.displayFast(); // draw a ball and show intermediate results with a partial update
  }
  lcd.setCursor(0, 112);
  lcd.print("then a partial update");
  lcd.displayFast(); // final partial update
  delay(4000);
  lcd.fillScreen(OBD_WHITE);
  lcd.setFreeFont(&Roboto_Black_40);
  lcd.setCursor(0,40);
  lcd.println("Gratuitous");
  lcd.println("pretty font");
  lcd.setFont(FONT_12x16);
  lcd.setCursor(0,112);
  lcd.println("Finished");
  lcd.display(); // show final screen with a full update
}

void loop() {
 // nothing going on here
}
