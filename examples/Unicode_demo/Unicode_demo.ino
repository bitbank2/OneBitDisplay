//
// Unicode character demo
//
// The purpose of this demo is to show how to use Unicode
// characters directly in your sketches with no hassles
// OneBitDisplay supports using 8-bit values to display
// TrueType font glyphs that have been converted to
// bitmap form. This limits the possible characters that
// can be displayed since Unicode is a 16-bit code. The
// workaround created by Microsoft many years ago was to
// select the most used characters for Western languages
// and place them in a set they called codepage 1252. This
// set includes accented characters and symbols in the
// range 128-255. These map to Unicode values up to 2300.
// OneBitDisplay now allows you to generate bitmap fonts
// with this mapping (use the fontconvert tool) and the
// UTF-8 multi-byte characters typed into your text editor
// get converted on the fly to the codepage 1252 range.
// This works for most situations except Asian and middle-East
// languages.
//
#include <OneBitDisplay.h>
#include "DejaVuSansMono_Bold14pt8b.h"
ONE_BIT_DISPLAY obd;

void setup()
{
  obd.I2Cbegin(OLED_128x64);
  obd.allocBuffer(); // we need a back buffer for precise vertical placement on this OLED
  obd.fillScreen(OBD_WHITE);
  obd.setFreeFont(&DejaVuSansMono_Bold14pt8b);
  obd.setCursor(0, 22); // the character baseline is the Y position for TrueType fonts
  obd.println("Está a"); // Use UTF-8 strings directly in your code
  obd.println("começar!"); // and the characters will map internally to codepage 1252
  obd.display(); // show the buffer on the physical display
}

void loop()
{

}
