//
// This demo sketch shows something that's normally very challenging
// to accomplish on a resource-constrained 8-bit MCU (Arduino Uno)
// The sketch has 2 TIFF G4 images (1-bit highly compressed)
// stored in FLASH that are decoded directly into the e-paper's
// frame buffer. This allows this particular demo to work with images
// that normally would not fit in the available RAM (2K). One limitation
// of this method is that the image can only be written in the native
// memory direction of the e-paper panel. In this case, the 128x296 EPD
// is oriented with the bytes in portrait orientation, MSB on the left.
//
// The G4 library has size limits imposed (200 pixels wide by N tall) to be
// able to use only 1K of RAM to accomplish this task. The OneBitDisplay
// library includes code specifically to allow working with e-paper
// displays without a local buffer to hold the pixels
//
// If you disable the Serial print statements, there will be
// plenty of FLASH space left on the UNO to hold other code/data
//
// Written by Larry Bank Oct 13, 2022
//
#include <TIFF_G4.h>
#include <OneBitDisplay.h>
#include "uno_bw.h"
#include "bart_1bpp.h"

ONE_BIT_DISPLAY obd;
TIFFG4 tiff;

//
// These defines are for the Waveshare Arduino UNO e-paper shield
// please change them to suit your hardware
//
#define EPD_FREQ 2000000
#define EPD_CS 10
#define EPD_DC 9
#define EPD_BUSY 7
#define EPD_RES 8
// Values of -1 here indicate that the default system pin numbers should be used
#define EPD_MOSI -1
#define EPD_SCK -1
// This is a 2.9" 128x296 e-paper with the SSD16xx controller chip
// The panel I used for this demo has "HINK-E029A27-A0"
#define EPD_TYPE EPD293_128x296

// This is the callback function you write to receive each line
// of decoded pixels as they're being decoded
void TIFFDraw(TIFFDRAW *pDraw)
{
//  Serial.print("TIFFDraw line = ");
//  Serial.println(pDraw->y, DEC);
  if (pDraw->y == 0) { // When you receive the first line, you can set the memory window parameters for the e-paper
    obd.setPosition(0, 0, tiff.getWidth(), tiff.getHeight());
  }
  // invert the pixels for EPD (black = 1)
  for (int i=0; i<(pDraw->iWidth/8); i++) {
     pDraw->pPixels[i] = ~pDraw->pPixels[i];
  }
  obd.pushPixels((uint8_t *)pDraw->pPixels, (pDraw->iWidth/8)); // send this line of pixels to the EPD; the destination address will automatically increment
} /* TIFFDraw() */

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  obd.setSPIPins(EPD_CS, EPD_MOSI, EPD_SCK, EPD_DC, EPD_RES, EPD_BUSY);
  obd.SPIbegin(EPD_TYPE, EPD_FREQ); // initialize OneBitDisplay for this e-paper panel
} /* setup() */

//
// This function will decode a TIFF image that is stored in FLASH
// given the starting offset and size of the data
//
void decodeImage(const uint8_t *pData, size_t size)
{
    if (tiff.openTIFF((uint8_t *)pData, (int)size, TIFFDraw))
    {
      Serial.print("TIFF opened, size = ");
      Serial.print(tiff.getWidth(), DEC);
      Serial.print("x");
      Serial.println(tiff.getHeight(), DEC);
      tiff.decode();
      tiff.close();
      Serial.println("Finished decoding");
    } else {
      Serial.print("Error opening TIFF = ");
      Serial.println(tiff.getLastError(), DEC);
    }
} /* decodeImage() */

void loop() {

  obd.fillScreen(OBD_WHITE); // clear the EPD's internal RAM to white
  obd.setTextColor(OBD_BLACK, OBD_WHITE); // use the correct EPD memory plane (0)
  decodeImage(uno_bw, sizeof(uno_bw));
  obd.display(); // this causes the EPD to do a full refresh
  obd.fillScreen(OBD_WHITE); // prepare the second image by clearing the EPD memory to white again
  decodeImage(bart_1bpp, sizeof(bart_1bpp));
  obd.display(); // show the Bart image
  obd.fillScreen(OBD_WHITE);
  obd.display(); // finish by clearing the EPD to white again
    while (1)
    {};
} /* loop() */
