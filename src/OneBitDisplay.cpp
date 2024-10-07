//
// OneBitDisplay (OLED/LCD/E-Paper library)
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
// This define (WIMPY_MCU) precludes the use of hardware interfaces
// such as I2C & SPI
//
#if defined (__AVR_ATtiny85__) || defined (ARDUINO_ARCH_MCS51)
#define WIMPY_MCU
#endif

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

#ifndef WIMPY_MCU
#include <SPI.h>
#endif

#endif // _LINUX_
#include "OneBitDisplay.h"
#include "obd_io.inl" // I/O (non-portable) code is in here
#include "obd.inl" // All of the display interface code is in here
#include "obd_gfx.inl" // drawing code
#ifdef __cplusplus
//
// C++ Class implementation
//
BBI2C * ONE_BIT_DISPLAY::getBB()
{
    return &_obd.bbi2c;
} /* getBB() */

uint32_t ONE_BIT_DISPLAY::getRefreshTime(void)
{
    return _obd.iTimeout;
}

void ONE_BIT_DISPLAY::setBB(BBI2C *pBB)
{
   memcpy(&_obd.bbi2c, pBB, sizeof(BBI2C));
} /* setBB() */

void ONE_BIT_DISPLAY::createVirtualDisplay(int width, int height, uint8_t *buffer)
{
   obdCreateVirtualDisplay(&_obd, width, height, buffer);
}

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
void ONE_BIT_DISPLAY::setPower(bool bOn)
{
    obdPower(&_obd, bOn);
} /* setPower() */

int ONE_BIT_DISPLAY::getFlags(void)
{
    return _obd.iFlags;
}

void ONE_BIT_DISPLAY::setFlags(int iFlags)
{
    _obd.iFlags = iFlags;
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

int ONE_BIT_DISPLAY::allocBuffer(void)
{
    int iSize = _obd.width * ((_obd.height+7)>>3);
    if (_obd.iFlags & (OBD_3COLOR | OBD_4COLOR) || _obd.chip_type == OBD_CHIP_UC8151)
        iSize *= 2; // 2 bit planes
    _obd.ucScreen = (uint8_t *)malloc(iSize);
    if (_obd.ucScreen != NULL) {
        _obd.render = false; // draw into RAM only
        memset(_obd.ucScreen, 0xff, iSize);
        return OBD_SUCCESS;
    }
    return OBD_ERROR_NO_MEMORY; // failed
} /* allocBuffer() */

uint32_t ONE_BIT_DISPLAY::capabilities(void)
{
  return _obd.iFlags;
}

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
  obdFill(&_obd, iColor, (_obd.ucScreen == NULL));
} /* fillScreen() */

void ONE_BIT_DISPLAY::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    obdRectangle(&_obd, x, y, x+w-1, y+h-1, color, 0);
} /* drawRect() */

void ONE_BIT_DISPLAY::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    obdRectangle(&_obd, x, y, x+w-1, y+h-1, color, 1);
} /* fillRect() */

void ONE_BIT_DISPLAY::setTextWrap(bool bWrap)
{
  _obd.wrap = (int)bWrap;
}

void ONE_BIT_DISPLAY::setTextColor(int iFG, int iBG)
{
    if (iFG > OBD_RED) iFG = OBD_BLACK;
    if (iBG > OBD_RED) iBG = OBD_BLACK;
    if ((_obd.iFlags & (OBD_3COLOR | OBD_4COLOR)) == 0) {
        if (iFG == OBD_RED) iFG = OBD_BLACK; // can't set red color
        if (iBG == OBD_RED) iBG = OBD_BLACK;
    }
    _obd.iFG = iFG;
    _obd.iBG = (iBG == -1) ? iFG : iBG;
} /* setTextColor() */

void ONE_BIT_DISPLAY::setCursor(int x, int y)
{
    if (x >= 0)
       _obd.iCursorX = x;
    if (y >= 0)
    _obd.iCursorY = y;
} /* setCursor() */

int ONE_BIT_DISPLAY::loadBMP(const uint8_t *pBMP, int x, int y, int iFG, int iBG)
{
    return obdLoadBMP(&_obd, pBMP, x, y, iFG, iBG);
} /* loadBMP() */

int ONE_BIT_DISPLAY::drawEPDGFX(int x, int y, int cx, int cy, uint8_t *pPlane0, uint8_t *pPlane1)
{
    return obdDrawEPDGFX(&_obd, x, y, cx, cy, pPlane0, pPlane1);
} /* drawEPDGFX() */

int ONE_BIT_DISPLAY::loadBMP3(const uint8_t *pBMP, int x, int y)
{
    return obdLoadBMP3(&_obd, pBMP, x, y);
} /* loadBMP3() */

void ONE_BIT_DISPLAY::setFont(int iFont)
{
    _obd.iFont = iFont;
    _obd.pFreeFont = NULL;
} /* setFont() */

void ONE_BIT_DISPLAY::setTextSize(uint8_t s)
{
    _obd.u32FontScaleX = _obd.u32FontScaleY = 256 * s;
    _obd.iFont = -1;
    _obd.pFreeFont = NULL;
}
void ONE_BIT_DISPLAY::setTextSize(uint8_t sx, uint8_t sy)
{
    _obd.u32FontScaleX = 256 * sx;
    _obd.u32FontScaleY = 256 * sy;
    _obd.iFont = -1;
    _obd.pFreeFont = NULL;
}
int ONE_BIT_DISPLAY::scrollBuffer(int iStartCol, int iEndCol, int iStartRow, int iEndRow, int bUp)
{
    return obdScrollBuffer(&_obd, iStartCol, iEndCol, iStartRow, iEndRow, bUp);
} /* scrollBuffer() */

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
#ifdef _LINUX_
void ONE_BIT_DISPLAY::print(const string &str)
{
   print(str.c_str());
} /* print() */

void ONE_BIT_DISPLAY::println(const string &str)
{
char ucTemp[4];

   print(str);
   ucTemp[0] = '\n';
   ucTemp[1] = '\r';
   ucTemp[2] = 0;
   print((const char *)ucTemp);
} /* print() */

void ONE_BIT_DISPLAY::print(const char *pString)
{
uint8_t *s = (uint8_t *)pString;

   while (*s != 0) {
      write(*s++);
   }
} /* print() */

void ONE_BIT_DISPLAY::println(const char *pString)
{
char ucTemp[4];

    print(pString);
    ucTemp[0] = '\n';
    ucTemp[1] = '\r';
    ucTemp[2] = 0;
    print((const char *)ucTemp);
} /* println() */
void ONE_BIT_DISPLAY::print(int value, int format)
{
char c, ucTemp[32];
char *d = &ucTemp[31];

   if (value) {
   d[0] = 0;
   switch(format) {
      case DEC:
         while (value) {
	     d--;
             *d = '0' + (value % 10);
             value /= 10;
	 }
         break;
      case HEX:
	 while (value) {
            d--;
            c = value & 0xf;
	    if (c < 10)
		    *d = '0' + c;
	    else
		    *d = 'A' + (c-10);
	    value >>= 4;
	 }
         break;
      case OCT:
	 while (value) {
            d--;
            *d = '0' + (value & 7);
	    value >>= 3;
	 }
         break;
      case BIN:
         while (value) {
            d--;
            *d = '0' + (value & 1);
	    value >>= 1;
	 }
         break;
      default:
         break;
      }
   } else { // if zero value
     d--;
     *d = '0';
   }
      print((const char *)d);
} /* print() */

void ONE_BIT_DISPLAY::println(int value, int format)
{
char ucTemp[4];

	print(value, format);
	ucTemp[0] = '\n';
	ucTemp[1] = '\r';
	ucTemp[2] = 0;
	print((const char *)ucTemp);
} /* println() */

#endif // _LINUX_
//
// write (Arduino Print friend class)
//
#ifndef __AVR__
size_t ONE_BIT_DISPLAY::write(uint8_t c) {
char szTemp[2]; // used to draw 1 character at a time to the C methods
int w, h;

  szTemp[0] = c; szTemp[1] = 0;
   if (_obd.pFreeFont == NULL) { // use built-in fonts
       if (_obd.iFont == -1) { // scaled 5x7 font
           h = (int)(_obd.u32FontScaleY >> 8) * 8;
           w = (int)(_obd.u32FontScaleX >> 8) * 6;
       } else if (_obd.iFont == FONT_8x8 || _obd.iFont == FONT_6x8) {
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
                obdDumpBuffer(&_obd, NULL, false, false, false);
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
                  obdDumpBuffer(&_obd, NULL, false, false, false);
              }
              _obd.iCursorY -= h;
          }
      }
        if (_obd.iFont == -1) { // scaled drawing
            obdScaledString(&_obd, _obd.iCursorX, _obd.iCursorY, szTemp, FONT_6x8, _obd.iFG, _obd.u32FontScaleX, _obd.u32FontScaleY, 0);
            _obd.iCursorX += w;
        } else {
            obdWriteString(&_obd, 0, -1, -1, szTemp, _obd.iFont, _obd.iFG, _obd.render);
        }
    }
  } else { // Custom font
    if (c == '\n') {
      _obd.iCursorX = 0;
      _obd.iCursorY += (uint8_t)pgm_read_byte(&_obd.pFreeFont->yAdvance);
    } else if (c != '\r') {
      uint8_t first = pgm_read_word((const uint8_t*)&_obd.pFreeFont->first);
      if ((c >= first) && (c <= (uint8_t)pgm_read_word((const uint8_t*)&_obd.pFreeFont->last))) {
        GFXglyph *glyph = pgm_read_glyph_ptr(_obd.pFreeFont, c - first);
        w = pgm_read_byte(&glyph->width);
        h = pgm_read_byte(&glyph->height);
        if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)pgm_read_word((const uint8_t*)&glyph->xOffset);
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
#endif // !__AVR__

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
void ONE_BIT_DISPLAY::wake(void)
{
    if (_obd.type >= EPD42_400x300) {
        EPDWakeUp(&_obd, 1);
    }
}
void ONE_BIT_DISPLAY::sleep(int bDeep)
{
    if (_obd.type >= EPD42_400x300) {
        EPDSleep(&_obd, bDeep);
    }
}

void ONE_BIT_DISPLAY::getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    if (_obd.pFreeFont) {
        int width, top, bottom;
        obdGetStringBox(_obd.pFreeFont, (char *)string, &width, &top, &bottom);
        *x1 = x;
        *w = width;
        *y1 = y + top;
        *h = (bottom - top + 1);
    }
}
void ONE_BIT_DISPLAY::getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    getTextBounds(str.c_str(), x, y, x1, y1, w, h);
}
int ONE_BIT_DISPLAY::dataTime(void)
{
    return _obd.iDataTime;
}
int ONE_BIT_DISPLAY::opTime(void)
{
    return _obd.iOpTime;
}
int16_t ONE_BIT_DISPLAY::width(void)
{
   return _obd.width;
}
int16_t ONE_BIT_DISPLAY::height(void)
{
   return _obd.height;
}
void ONE_BIT_DISPLAY::drawSprite(uint8_t *pSprite, int cx, int cy, int iPitch, int x, int y, uint8_t iPriority)
{
    obdDrawSprite(&_obd, pSprite, cx, cy, iPitch, x, y, iPriority);
}
int ONE_BIT_DISPLAY::drawGFX(uint8_t *pSrc, int iSrcCol, int iSrcRow, int iDestCol, int iDestRow, int iWidth, int iHeight, int iSrcPitch)
{
    return obdDrawGFX(&_obd, pSrc, iSrcCol, iSrcRow, iDestCol, iDestRow, iWidth, iHeight, iSrcPitch);
} /* drawGFX() */

void ONE_BIT_DISPLAY::drawTile(const uint8_t *pTile, int x, int y, int iRotation, int bInvert, int bRender)
{
    obdDrawTile(&_obd, pTile, x, y, iRotation, bInvert, bRender);
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
void ONE_BIT_DISPLAY::setPosition(int x, int y, int w, int h)
{
    if (_obd.type >= EPD42_400x300)
        EPDSetPosition(&_obd, x, y, w, h);
    else
        obdSetPosition(&_obd, x, y, 1);
} /* setPosition() */
void ONE_BIT_DISPLAY::writeRaw(uint8_t *pData, int iLen)
{
   RawWrite(&_obd, pData, iLen);
}

void ONE_BIT_DISPLAY::writeCommand(uint8_t ucCMD)
{
   obdWriteCommand(&_obd, ucCMD);
}
void ONE_BIT_DISPLAY::pushPixels(uint8_t *pPixels, int iCount)
{
    RawWriteData(&_obd, pPixels, iCount);
} /* pushPixels() */

void ONE_BIT_DISPLAY::pushImage(int x, int y, int w, int h, uint16_t *pixels)
{
    // DEBUG
    (void)x; (void)y; (void)w; (void)h; (void)pixels;
}

void ONE_BIT_DISPLAY::displayLines(int iStartLine, int iLineCount)
{
    obdWriteLCDLines(&_obd, iStartLine, iLineCount);
} /* displayLines() */

int ONE_BIT_DISPLAY::display(bool bRefresh, bool bWait, bool bFast)
{
    return obdDumpBuffer(&_obd, NULL, bRefresh, bWait, bFast);
}

void ONE_BIT_DISPLAY::wait(bool bQuick)
{
    EPDWaitBusy(&_obd, bQuick);
}
int ONE_BIT_DISPLAY::displayPartial()
{
    if (_obd.type >= EPD42_400x300 && (_obd.iFlags & OBD_HAS_PARTIAL_UPDATE)) {
        return EPDDumpPartial(&_obd, NULL, 0, 0, _obd.width, _obd.height);
    }
    return OBD_ERROR_NOT_SUPPORTED;
}
int ONE_BIT_DISPLAY::displayPartial(int x, int y, int w, int h, uint8_t *pBuffer)
{
    if (_obd.type >= EPD42_400x300 && (_obd.iFlags & OBD_HAS_PARTIAL_UPDATE)) {
        return obdDumpPartial(&_obd, x, y, w, h, pBuffer);
    } else {
        return OBD_ERROR_NOT_SUPPORTED;
    }
}
void ONE_BIT_DISPLAY::drawString(const char *pText, int x, int y)
{
    setCursor(x,y);
    if (_obd.pFreeFont != NULL)
        obdWriteStringCustom(&_obd, _obd.pFreeFont, x, y, (char *)pText, _obd.iFG);
    else
        obdWriteString(&_obd, 0, x, y, (char *)pText, _obd.iFont, _obd.iFG, 1);
} /* drawString() */
void ONE_BIT_DISPLAY::drawString(String text, int x, int y)
{
    drawString(text.c_str(), x, y);
} /* drawString() */

void ONE_BIT_DISPLAY::backlight(int bOn)
{
    obdBacklight(&_obd, bOn);
}
OBDISP * ONE_BIT_DISPLAY::getOBD()
{
    return &_obd;
}
void ONE_BIT_DISPLAY::setRender(bool bRAMOnly)
{
    _obd.render = !bRAMOnly;
}
#endif // __cplusplus
