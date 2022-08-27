//
// Inky Phat (old) test
// 104x212 e-paper (SSD1608)
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <armbianio.h>
#include "../../src/OneBitDisplay.h"
//#include "Roboto_Black_40.h"
//#include "Roboto_Black_20.h"

ONE_BIT_DISPLAY epd;

#define EPD_FREQ 2000000
// Waveshare 2.13 250x122 hat
#define INKY_CS 24
#define INKY_BUSY 18
#define INKY_DC 22
#define INKY_RES 11

//#define INKY_CS 27
//#define INKY_BUSY 11
//#define INKY_DC 15
//#define INKY_RES 13
#define INKY_MOSI -1
#define INKY_SCK -1

int main(int argc, char **argv)
{
  AIOInitBoard("Raspberry Pi");
  printf("Starting...\n");
  epd.setSPIPins(INKY_CS, -1, -1, INKY_DC, INKY_RES, INKY_BUSY);
  epd.SPIbegin(EPD213_104x212, EPD_FREQ);
  epd.setRotation(270);
  epd.allocBuffer();
  epd.fillScreen(0);

  epd.setFont(FONT_12x16);
  epd.println("Inky Phat with");
  epd.println("OneBitDisplay lib");
  epd.setTextColor(OBD_RED);
  epd.println(1234, DEC);
  epd.println(4095, HEX);
  epd.display();
  printf("Display done\n");
  return 0;
} /* main() */
