//
// OneBitDisplay compressed image example
//
#include <OneBitDisplay.h>
#include "bart_128x64.h"
#include "smiley.h" // 100x100
ONE_BIT_DISPLAY obd;

void setup() {
int rc;
float f;
  obd.I2Cbegin(OLED_128x64);
  obd.fillScreen(OBD_WHITE);
  // OneBitDisplay can draw compressed graphics directly onto a display
  // without any local framebuffer, but there are limitations
  for (int i=0; i<50; i++) {
      f = 0.02f * (float)i;
      // Draw the Bart image scaled and inverted
      rc = obd.loadG5Image(bart_128x64, 0, 0, OBD_WHITE, OBD_BLACK, f);
  }
  delay(3000);
  // Setting the foreground and background colors equal tells it to draw the background as transparent
  // but... it can only do that when drawing into memory
  obd.allocBuffer(); // allocate a framebuffer
  obd.fillScreen(OBD_WHITE);
  obd.loadG5Image(bart_128x64, 0, 0, OBD_WHITE, OBD_BLACK); // draw at 1.0 scale
 // Draw the 100x100 smiley as 50x50 transparently on top of Bart
  obd.loadG5Image(smiley, (128-50)/2, (64-50)/2, OBD_WHITE, OBD_WHITE, 0.5f);
// Since we're using a back buffer, we need to tell OneBitDisplay to
// explicitly copy the pixels from memory to the physical display
  obd.display();
} /* setup() */

void loop()
{

}