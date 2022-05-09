//
// OneBitDisplay (OLED+LCD library)
// Copyright (c) 2020 BitBank Software, Inc.
// Written by Larry Bank (bitbank@pobox.com)
// Project started 3/23/2020
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifdef _LINUX_
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#ifndef MEMORY_ONLY
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <armbianio.h>
#endif // MEMORY_ONLY
// convert wire library constants into ArmbianIO values
#define OUTPUT GPIO_OUT
#define INPUT GPIO_IN
#define INPUT_PULLUP GPIO_IN_PULLUP
#define HIGH 1
#define LOW 0
void delay(int);
#else // Arduino

#include <Arduino.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif
#include <SPI.h>

#endif // _LINUX_
#include "OneBitDisplay.h"
// All of the drawing code is in here
#include "obd.inl"

//
// C++ Class implementation
//
void ONE_BIT_DISPLAY::SPIbegin(int iType, int32_t iSpeed)
{
    obdSPIInit(&_obd, iType, _obd.iDCPin, _obd.iCSPin, _obd.iRSTPin, _obd.iMOSIPin, _obd.iCLKPin, _obd.iLEDPin, _obd.flip, _obd.invert, _obd.bBitBang, iSpeed);
} /* SPIbegin() */

void ONE_BIT_DISPLAY::setSPIPins(int iCS, int iMOSI, int iSCLK, int iDC, int iReset, int iLED)
{
    _obd.iCSPin = iCS;
    _obd.iMOSIPin = iMOSI;
    _obd.iCLKPin = iSCLK;
    _obd.iDCPin = iDC;
    _obd.iRSTPin = iReset;
    _obd.iLEDPin = iLED;
} /* setSPIPins() */

void ONE_BIT_DISPLAY::setI2CPins(int iSDA, int iSCL, int iReset)
{
    _obd.iSDAPin = iSDA;
    _obd.iSCLPin = iSCL;
    _obd.iRSTPin = iReset;
}
void ONE_BIT_DISPLAY::setBitBang(bool bBitBang)
{
    _obd.bBitBang = bBitBang;
}
void ONE_BIT_DISPLAY::setFlags(int iFlags)
{
    _obd.invert = iFlags & OBD_INVERTED;
    _obd.flip = iFlags & OBD_FLIP180;
}

void ONE_BIT_DISPLAY::setContrast(uint8_t ucContrast)
{
  obdSetContrast(&_obd, ucContrast);
}

int ONE_BIT_DISPLAY::I2Cbegin(int iType, int iAddr, int32_t iSpeed)
{
  return obdI2CInit(&_obd, iType, iAddr, _obd.flip, _obd.invert, !_obd.bBitBang, _obd.iSDAPin, _obd.iSCLPin, _obd.iRSTPin, iSpeed);
} /* I2Cbegin() */

void ONE_BIT_DISPLAY::setBuffer(uint8_t *pBuffer)
{
    _obd.ucScreen = pBuffer;
}

bool ONE_BIT_DISPLAY::allocBuffer(void)
{
    int iSize = _obd.width * ((_obd.height+7)>>3);
    _obd.ucScreen = (uint8_t *)malloc(iSize);
    if (_obd.ucScreen != NULL) {
        _obd.render = false; // draw into RAM only
        return 1;
    }
    return 0; // failed
} /* allocBuffer() */

void * ONE_BIT_DISPLAY::getBuffer(void)
{
    return (void *)_obd.ucScreen;
} /* getBuffer() */

void ONE_BIT_DISPLAY::freeBuffer(void)
{
    if (_obd.ucScreen) {
        free(_obd.ucScreen);
        _obd.ucScreen = NULL;
    }
} /* freeBuffer() */

void ONE_BIT_DISPLAY::setScroll(bool bScroll)
{
    _obd.bScroll = bScroll;
}

void ONE_BIT_DISPLAY::setRotation(int iRotation)
{
    obdSetRotation(&_obd, iRotation);
} /* setRotation() */

void ONE_BIT_DISPLAY::fillScreen(int iColor)
{
  obdFill(&_obd, iColor, 1);
} /* fillScreen() */

void ONE_BIT_DISPLAY::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    obdRectangle(&_obd, x, y, x+w-1, y+h-1, color, 0);
} /* drawRect() */

void ONE_BIT_DISPLAY::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    obdRectangle(&_obd, x, y, x+w-1, y+h-1, color, 1);
} /* fillRect() */

void ONE_BIT_DISPLAY::setTextColor(int iFG, int iBG)
{
  _obd.iFG = iFG;
  _obd.iBG = (iBG == -1) ? iFG : iBG;
} /* setTextColor() */

void ONE_BIT_DISPLAY::setCursor(int x, int y)
{
    _obd.iCursorX = x;
    _obd.iCursorY = y;
} /* setCursor() */

void ONE_BIT_DISPLAY::setFont(int iFont)
{
    _obd.iFont = iFont;
    _obd.pFreeFont = NULL;
} /* setFont() */

void ONE_BIT_DISPLAY::setFreeFont(const GFXfont *pFont)
{
    _obd.pFreeFont = (GFXfont *)pFont;
} /* setFreeFont() */

void ONE_BIT_DISPLAY::drawLine(int x1, int y1, int x2, int y2, int iColor)
{
    obdDrawLine(&_obd, x1, y1, x2, y2, iColor, 1);
} /* drawLine() */

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_ptr(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned
  // pointer will break strict-aliasing rules" warning In fact, on other
  // platforms (such as STM32) there is no need to do this pointer magic as
  // program memory may be read in a usual way So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

void obdScroll1Line(OBDISP *pOBD, int iAmount)
{
int y, iLines;

    if (pOBD == NULL || pOBD->ucScreen == NULL)
        return;
    iLines = (pOBD->height+7)>>3;
    for (y=0; y<iLines-iAmount; y+=iAmount) {
        memcpy(&pOBD->ucScreen[y*pOBD->width], &pOBD->ucScreen[(y+iAmount)*pOBD->width], pOBD->width*iAmount);
    }
    memset(&pOBD->ucScreen[(iLines-iAmount) * pOBD->width], (char)pOBD->iBG, pOBD->width*iAmount);
} /* obdScroll1Line() */
//
// write (Print friend class)
//
size_t ONE_BIT_DISPLAY::write(uint8_t c) {
char szTemp[2]; // used to draw 1 character at a time to the C methods
int w, h;

  szTemp[0] = c; szTemp[1] = 0;
  if (_obd.pFreeFont == NULL) { // use built-in fonts
      if (_obd.iFont == FONT_8x8 || _obd.iFont == FONT_6x8) {
        h = 8;
        w = (_obd.iFont == FONT_8x8) ? 8 : 6;
      } else if (_obd.iFont == FONT_12x16 || _obd.iFont == FONT_16x16) {
        h = 16;
        w = (_obd.iFont == FONT_12x16) ? 12 : 16;
      } else { w = 16; h = 32; }

    if (c == '\n') {              // Newline?
      _obd.iCursorX = 0;          // Reset x to zero,
      _obd.iCursorY += h; // advance y one line
        // should we scroll the screen up 1 line?
        if (_obd.iCursorY >= _obd.height && _obd.ucScreen && _obd.bScroll) {
            obdScroll1Line(&_obd, h/8);
            if (_obd.render) {
                obdDumpBuffer(&_obd, NULL);
            }
            _obd.iCursorY -= h;
        }
    } else if (c != '\r') {       // Ignore carriage returns
      if (_obd.wrap && ((_obd.iCursorX + w) > _obd.width)) { // Off right?
        _obd.iCursorX = 0;               // Reset x to zero,
        _obd.iCursorY += h; // advance y one line
          // should we scroll the screen up 1 line?
          if (_obd.iCursorY >= _obd.height && _obd.ucScreen && _obd.bScroll) {
              obdScroll1Line(&_obd, h/8);
              if (_obd.render) {
                  obdDumpBuffer(&_obd, NULL);
              }
              _obd.iCursorY -= h;
          }
      }
      obdWriteString(&_obd, 0, -1, -1, szTemp, _obd.iFont, !_obd.iFG, _obd.render);
    }
  } else { // Custom font
    if (c == '\n') {
      _obd.iCursorX = 0;
      _obd.iCursorY += (uint8_t)pgm_read_byte(&_obd.pFreeFont->yAdvance);
    } else if (c != '\r') {
      uint8_t first = pgm_read_byte(&_obd.pFreeFont->first);
      if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&_obd.pFreeFont->last))) {
        GFXglyph *glyph = pgm_read_glyph_ptr(_obd.pFreeFont, c - first);
        w = pgm_read_byte(&glyph->width);
        h = pgm_read_byte(&glyph->height);
        if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset);
          w += xo; // xadvance
          h = (uint8_t)pgm_read_byte(&_obd.pFreeFont->yAdvance);
          if (_obd.wrap && ((_obd.iCursorX + w) > _obd.width)) {
            _obd.iCursorX = 0;
            _obd.iCursorY += h;
          }
            obdWriteStringCustom(&_obd, _obd.pFreeFont, -1, -1, szTemp, _obd.iFG);
        }
      }
    }
  }
  return 1;
} /* write() */
void ONE_BIT_DISPLAY::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    obdSetPixel(&_obd, x, y, color, _obd.render);
}
int16_t ONE_BIT_DISPLAY::getCursorX(void)
{
  return _obd.iCursorX;
}
int16_t ONE_BIT_DISPLAY::getCursorY(void)
{
  return _obd.iCursorY;
}
uint8_t ONE_BIT_DISPLAY::getRotation(void)
{
  return _obd.iOrientation;
}
int16_t ONE_BIT_DISPLAY::width(void)
{
   return _obd.width;
}
int16_t ONE_BIT_DISPLAY::height(void)
{
   return _obd.height;
}
void ONE_BIT_DISPLAY::drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color)
{
  obdEllipse(&_obd, x, y, r, r, color, 0);
}
void ONE_BIT_DISPLAY::fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color)
{
    obdEllipse(&_obd, x, y, r, r, color, 1);
}
void ONE_BIT_DISPLAY::drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color)
{
    obdEllipse(&_obd, x, y, rx, ry, color, 0);
}
void ONE_BIT_DISPLAY::fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color)
{
    obdEllipse(&_obd, x, y, rx, ry, color, 1);
}

void ONE_BIT_DISPLAY::pushImage(int x, int y, int w, int h, uint16_t *pixels)
{
    // DEBUG
    (void)x; (void)y; (void)w; (void)h; (void)pixels;
}

void ONE_BIT_DISPLAY::display(void)
{
    obdDumpBuffer(&_obd, NULL);
}
void ONE_BIT_DISPLAY::setRender(bool bRAMOnly)
{
    _obd.render = !bRAMOnly;
}
