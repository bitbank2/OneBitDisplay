//
// Inky Phat (old black+white version) test
// 104x212 e-paper (SSD1608)
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <armbianio.h>
#include "../../src/OneBitDisplay.cpp"

OBDISP epd;

#define EPD_FREQ 400000
// header pin numbers, NOT BCM
#define INKY_CS 27
#define INKY_BUSY 11
#define INKY_DC 15
#define INKY_RES 13
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

  obdSPIInit(&epd, EPD213_104x212, INKY_DC, INKY_CS, INKY_RES, -1, -1, INKY_BUSY, FLIP180, INVERT, BITBANG, EPD_FREQ);
  obdSetRotation(&epd, 90);
  obdAllocBuffer(&epd);
  obdFill(&epd, 0, 0);
  obdWriteString(&epd, 0,0,0,(char *)"Inky Phat with", FONT_12x16, 0, 0);
  obdWriteString(&epd, 0,0,16,(char *)"OneBitDisplay lib", FONT_12x16, 0, 0);
  for (i=32; i<128; i+=8) {
     char szTemp[32];
     sprintf(szTemp, "%d", i);
     obdWriteString(&epd, 0,0,i, szTemp, FONT_8x8, 0,0);
  }
  obdDumpBuffer(&epd, NULL);
  printf("Display done\n");
  return 0;
} /* main() */

