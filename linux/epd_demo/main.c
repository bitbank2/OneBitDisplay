//
// WaveShare 2.7" e-paper HAT demo
// The same hardware connection numbers should work
// on other WaveShare hats
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <armbianio.h>
#include <OneBitDisplay.h>
//#include "../../src/OneBitDisplay.cpp"
#include "Roboto_Black_28.h"

OBDISP epd;

#define EPD_FREQ 4000000
// header pin numbers, NOT BCM
#define INKY_CS 24
#define INKY_BUSY 18
#define INKY_DC 22
#define INKY_RES 11
#define FLIP180 0
#define INVERT 0
#define BITBANG 0

int main(int argc, char **argv)
{
int i;

  printf("Starting...\n");
  i = AIOInitBoard("Raspberry Pi");
  if (i == 0) // problem
  {
     printf("Error in AIOInitBoard(); check if this board is supported\n");
     return 0;
  }

  obdSPIInit(&epd, EPD27_176x264, INKY_DC, INKY_CS, INKY_RES, -1, -1, INKY_BUSY, FLIP180, INVERT, BITBANG, EPD_FREQ);
  obdSetRotation(&epd, 90);
  obdAllocBuffer(&epd);
  obdFill(&epd, 0, 0);
  obdWriteString(&epd, 0,0,0,(char *)"WaveShare 2.7\" e-paper", FONT_12x16, OBD_BLACK, 0);
  obdWriteString(&epd, 0,0,16,(char *)"OneBitDisplay lib in C", FONT_12x16,OBD_BLACK,0);
  obdEllipse(&epd,epd.width/2, epd.height/2, 40,40,OBD_BLACK,1);
  obdWriteStringCustom(&epd, (GFXfont *)&Roboto_Black_28, 0,epd.height, (char *)"Custom fonts too", OBD_BLACK);
  obdDumpBuffer(&epd, NULL); // do a full update
  printf("Display done\n");
  return 0;
} /* main() */

