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

#include "../../../AnimatedGIF/src/AnimatedGIF.h"
#include "../../../AnimatedGIF/src/gif.inl"
#include "../../src/OneBitDisplay.cpp"
#include "running_256_64.h"

#define OLED_TYPE OLED_132x64
#define OLED_ADDR 0x3c
#define FLIP180 0
#define INVERT 0
#define HW_I2C 1
// On Linux this is the I2C bus number (SDA_PIN) for HW I2C
#define SDA_PIN 1
// On Linux this is the I2C device address
#define SCL_PIN 0x3c
#define RESET_PIN -1
// This is not controlled on Linux through the API
// On the Raspberry PI, see /boot/config.txt
#define SPEED 0
#define DISPLAY_WIDTH 256
#define DISPLAY_HEIGHT 64
static uint8_t ucOLED_0[1024], ucOLED_1[1024];
static OBDISP obd[2];
//
// This doesn't have to be super efficient
//
void DrawPixel(int x, int y, uint8_t ucColor)
{
uint8_t *d, ucMask;

  ucMask = 1 << (y & 7);
  if (x >= 128) // right display
     d = &ucOLED_1[(x-128)+((y>>3) << 7)];
  else
     d = &ucOLED_0[x + ((y>>3) << 7)];
  if (ucColor)
     *d |= ucMask;
  else
     *d &= ~ucMask;
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
    if (pDraw->y == pDraw->iHeight-1) { // last line, render it to the display(s)
       obdDumpBuffer(&obd[0], NULL);
       obdDumpBuffer(&obd[1], NULL);
    }
} /* GIFDraw() */

int main(int argc, const char * argv[])
{
GIFIMAGE gif;
int rc;
int iDelay;

    obdI2CInit(&obd[0], OLED_TYPE, -1, FLIP180, INVERT, HW_I2C, 1, 0x3C, RESET_PIN, SPEED);
    obdSetBackBuffer(&obd[0], ucOLED_0);
    obdFill(&obd[0], 0, 1);
    obdI2CInit(&obd[1], OLED_TYPE, -1, FLIP180, INVERT, HW_I2C, 1, 0x3D, RESET_PIN, SPEED);
    obdSetBackBuffer(&obd[1], ucOLED_1);
    obdFill(&obd[1], 0, 1);
    GIF_begin(&gif, GIF_PALETTE_RGB565_LE);
    while (1) {
       rc = GIF_openRAM(&gif, (uint8_t *)running_256_64, sizeof(running_256_64), GIFDraw);
       if (rc) {
          while (GIF_playFrame(&gif, &iDelay, NULL)) {
            // usleep(iDelay * 1000);
	  }	  
       }
    } // while (1)
    return 0;
} /* main() */
