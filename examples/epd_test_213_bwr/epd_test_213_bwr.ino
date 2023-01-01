//
// OneBitDisplay BWR EPD (e-ink) demo sketch
// Written by Larry Bank 7/10/2022
// edit by Aaron Christophel ATC1441 20/11/2022
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

#include <OneBitDisplay.h>

ONE_BIT_DISPLAY lcd;

#define EPD_FREQ 12000000
#define EPD_CS 5
#define EPD_BUSY 4
#define EPD_RES 16
#define EPD_DC 17
#define EPD_MOSI 23
#define EPD_SCK 18

void setup() {
  // Initialize the library by telling it how the display is connected
  lcd.setSPIPins(EPD_CS, -1, -1, EPD_DC, EPD_RES, EPD_BUSY);
  lcd.SPIbegin(EPD213R_104x212_d, EPD_FREQ); // 12Mhz is fast enough

  // Set the orientation to draw in the direction we want
  lcd.setRotation(90); // the panel is actually 128 wide by 296 tall, but oriented 90 right
  lcd.allocBuffer(); // allocate a back buffer and keep it internal to the class
  lcd.fillScreen(OBD_WHITE); // 0 for EPDs is white (for OneBitDisplay)
  lcd.setFont(FONT_12x16); // use the internal stretched/smoothed font
  lcd.println("This was");
  lcd.setTextColor(2);
  lcd.print("a full update");
  lcd.display(); // do a full update
}

void loop() {
 // nothing going on here
}
