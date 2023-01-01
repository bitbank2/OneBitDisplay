//
// Simple demo sketch to show how to use the new
// C++ class which wraps the OneBitDisplay C API
// This class was modeled after the Adafruit GFX API
// to make it easier to port code, yet still retains
// the unique features of OBD
//
// Written by Larry Bank
// project started April 24, 2022
//
#include <OneBitDisplay.h>
ONE_BIT_DISPLAY tft; // static class instantiation

void setup()
{
  int i;
  // The I2Cbegin() method needs a minimum of the type of OLED/LCD being used
  // Optional parameters are for the I2C address (auto discovered if not specified)
  // and the I2C bus speed (defaults to 400Kbs)
  // The Arduino default I2C pins are used here (works on many setups)
  // If you need to specify them, use the setI2CPins() method
  tft.I2Cbegin(OLED_64x128);
  //
  // Here we're asking the library to allocate the backing buffer
  // If successful, the library will change the render flag to "RAM only" so that
  // all drawing occurs only to the internal buffer, and display() must be called
  // to see the changes. Without a backing buffer, the API will try to draw all
  // output directly to the display instead.
  //
  if (!tft.allocBuffer()) {
    tft.print("Alloc failed");
  }
//  tft.setRotation(3); // optionally rotate in 90 degree increments - only supports 0/180 without a RAM backing buffer
  tft.fillScreen(OBD_WHITE);
  tft.setScroll(true); // enable text printing to scroll the display buffer a line at a time
  tft.setFont(FONT_12x16); // Use the 6x8 (stretched+smoothed) font
  i = 0;
  while (1) {
    tft.print("Count = ");
    tft.println(i++, DEC); // this will keep the last line blank because it immediately scrolls up
    tft.display(); // copy the RAM buffer to the physical display
    delay(250); // slow it down a bit
  }
}

void loop() {
// nothing going on here
}
