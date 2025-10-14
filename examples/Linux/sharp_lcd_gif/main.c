//
//  main.c
//  OneBitDisplay library test program
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <armbianio.h>
#include <signal.h>

#include "../../../AnimatedGIF/src/AnimatedGIF.h"
#include "../../../AnimatedGIF/src/gif.inl"
#include "../../src/OneBitDisplay.cpp"
#include "pattern_400x240.h"

#define DISPLAY_TYPE SHARP_400x240
#define FLIP180 0
#define INVERT 0
#define DC_PIN -1
#define CS_PIN 16
#define DISP_PIN 18
#define EXTCOM_PIN 22
#define RESET_PIN -1
#define SPEED 8000000
#define BITBANG 0
#define LED_PIN -1

// This is not controlled on Linux through the API
// On the Raspberry PI, see /boot/config.txt
#define DISPLAY_WIDTH 400 
#define DISPLAY_HEIGHT 240
static uint8_t ucBuffer[12000];
static OBDISP obd;
volatile int iStop = 0;

void my_handler(int signal)
{
   iStop = 1;
} /* my_handler() */

//
// This doesn't have to be super efficient
//
void DrawPixel(int x, int y, uint8_t ucColor)
{
uint8_t *d, ucMask;

  ucMask = 1 << (y & 7);
  d = &ucBuffer[x + ((y>>3) * DISPLAY_WIDTH)];
  if (ucColor)
     *d &= ~ucMask;
  else
     *d |= ucMask;
}

// Draw a line of image into our 1-bpp virtual display buffers
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    int x, y, iWidth;
    static uint8_t ucPalette[256]; // thresholded palette

    if (pDraw->y == 0) // first line, convert palette to 0/1
    {
      for (x = 0; x < 256; x++)
      {
        uint16_t usColor = pDraw->pPalette[x];
        int gray = (usColor & 0xf800) >> 8; // red
        gray += ((usColor & 0x7e0) >> 2); // plus green*2
        gray += ((usColor & 0x1f) << 3); // plus blue
        ucPalette[x] = (gray >> 9); // 0->511 = 0, 512->1023 = 1
      }
    }
    y = pDraw->iY + pDraw->y; // current line
    iWidth = pDraw->iWidth;
    if (iWidth > DISPLAY_WIDTH)
       iWidth = DISPLAY_WIDTH;

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t c, ucTransparent = pDraw->ucTransparent;
      int x;
      for(x=0; x < iWidth; x++)
      {
        c = *s++;
        if (c != ucTransparent)
             DrawPixel(pDraw->iX + x, y, ucPalette[c]);
      }
    }
    else
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
        DrawPixel(pDraw->iX + x, y, ucPalette[*s++]);
    }
    if (pDraw->y == pDraw->iHeight-1) { // last line, render it to the display
       obdDumpBuffer(&obd, NULL);
    }
} /* GIFDraw() */

int main(int argc, const char * argv[])
{
GIFIMAGE gif;
int rc;
int iDelay;
struct sigaction sigIntHandler;

// Set CTRL-C signal handler
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    printf("Sharp Memory LCD GIF demo\n");
    printf("Press Ctrl-C to quit\n");

    rc = AIOInitBoard("Raspberry Pi");
    if (rc == 0) // problem
    {
       printf("Error in AIOInit(); check if this board is supported\n");
       return 0;
    }
    AIOAddGPIO(DISP_PIN, OUTPUT); // enable display
    AIOWriteGPIO(DISP_PIN, HIGH);
    AIOAddGPIO(EXTCOM_PIN, OUTPUT);

    obdSPIInit(&obd, DISPLAY_TYPE, DC_PIN, CS_PIN, RESET_PIN, -1, -1, LED_PIN, FLIP180, INVERT, BITBANG, SPEED);
    obdSetBackBuffer(&obd, ucBuffer);
    obdFill(&obd, 0, 0);
    obdDumpBuffer(&obd, NULL);
    GIF_begin(&gif, GIF_PALETTE_RGB565_LE);
    while (!iStop) {
       rc = GIF_openRAM(&gif, (uint8_t *)pattern_400x240, sizeof(pattern_400x240), GIFDraw);
       if (rc) {
          while (GIF_playFrame(&gif, &iDelay, NULL)) {
            //usleep(iDelay * 1000);
	  }	  
       } else {
	  printf("GIF decode error = %d\n", gif.iError);
	  iStop = 1;
       }
    } // while (1)
    AIOWriteGPIO(DISP_PIN, LOW); // turn off LCD
    AIOShutdown();

    return 0;
} /* main() */
