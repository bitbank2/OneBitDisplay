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
#ifndef __ONEBITDISPLAY__
#define __ONEBITDISPLAY__

#if !defined( MEMORY_ONLY ) && defined(ARDUINO)
#include <BitBang_I2C.h>
#endif

// For Linux and esp-idf we add a file/device handle member
// to the BBI2C structure
#ifndef ARDUINO
typedef struct _tagbbi2c
{
  int file_i2c;
  uint8_t iSDA, iSCL;
  uint8_t bWire;
} BBI2C;
#endif

#ifdef __LINUX__
// for Print support
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define PROGMEM
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif // __LINUX__

// error messages
enum {
    OBD_SUCCESS,
    OBD_ERROR_BAD_PARAMETER,
    OBD_ERROR_BAD_DATA,
    OBD_ERROR_NOT_SUPPORTED,
    OBD_ERROR_NO_MEMORY,
    OBD_ERROR_COUNT
};

// controller chip types
enum {
    OBD_CHIP_NA = 0,
    OBD_CHIP_SSD13xx, // OLED
    OBD_CHIP_SH11xx, // OLED
    OBD_CHIP_SHARP, // all Sharp LCDs
    OBD_CHIP_COUNT
};

// 5 possible font sizes: 8x8, 16x32, 6x8, 12x16 (stretched from 6x8 with smoothing), 16x16 (stretched from 8x8)
enum {
   FONT_6x8 = 0,
   FONT_8x8,
   FONT_12x16,
   FONT_16x16,
   FONT_16x32,
   FONT_COUNT
};

// For backwards compatibility, keep the old names valid
#define FONT_NORMAL FONT_8x8
#define FONT_SMALL FONT_6x8
#define FONT_STRETCHED FONT_16x16
#define FONT_LARGE FONT_16x32
#endif

// Display type for init function
enum {
  DISPLAY_COMMANDS = 0,
  OLED_128x128,
  OLED_128x32,
  OLED_128x64,
  OLED_132x64,
  OLED_64x128,
  OLED_64x32,
  OLED_64x48,
  OLED_96x16,
  OLED_72x40,
  OLED_80x128,
  LCD_UC1701,
  LCD_UC1609,
  LCD_HX1230,
  LCD_32x64,
  LCD_128x64_I2C,
  LCD_NOKIA5110,
  LCD_VIRTUAL,
  SHARP_128x128,
  SHARP_144x168,
  SHARP_400x240,
  SHARP_160x68,
#ifndef __AVR__
  LCD_ST7302,
  LCD_ST7305,
#endif
  LCD_COUNT
};

// flag bits
#define OBD_INVERTED 0x0001
#define OBD_FLIP180  0x0002  
#define OBD_FLIPV    0x0004
#define OBD_FLIPH    0x0008
#define OBD_BITBANG  0x0010

#define OBD_WHITE 0
#define OBD_BLACK 1

#define OBD_ANY_ADDRESS -1
// Rotation and flip angles to draw tiles
enum {
  ANGLE_0=0,
  ANGLE_90,
  ANGLE_180,
  ANGLE_270,
  ANGLE_FLIPX,
  ANGLE_FLIPY
};

typedef struct {
    int x; 
    int y;
    int w;
    int h;
} BB_RECT; 

typedef struct obdstruct
{
#ifndef MEMORY_ONLY
BBI2C bbi2c;
#endif
uint8_t oled_addr; // requested address or 0xff for automatic detection
uint8_t busy_idle, wrap, flip, invert, type, render, can_flip, chip_type;
uint8_t *ucScreen;
int iCursorX, iCursorY;
int width, height, native_width, native_height;
uint8_t bScroll;
int iScreenOffset, iOrientation;
int iFG, iBG; //current color
int iFont, iFlags;
int iDataTime, iOpTime; // time in milliseconds for data transmission and operation
uint32_t u32FontScaleX, u32FontScaleY;
uint32_t iSpeed;
uint32_t iTimeout; // for e-ink panels
void *pFont;
uint8_t com_mode; // communication mode (I2C / SPI)
uint8_t mode; // data/command mode for 9-bit SPI
uint8_t iSDAPin, iSCLPin;
uint8_t iDCPin, iMOSIPin, iCLKPin, iCSPin, iRSTPin;
uint8_t x_offset, y_offset; // memory offsets
int iLEDPin; // backlight
uint8_t bBitBang;
} OBDISP;

#ifdef __cplusplus
#ifdef __LINUX__
#include <string>
using namespace std;
class ONE_BIT_DISPLAY
#else // Arduino
#ifndef __AVR__
class ONE_BIT_DISPLAY : public Print
#else
class ONE_BIT_DISPLAY
#endif // !__AVR__
#endif // __LINUX__
{
  public:
    ONE_BIT_DISPLAY() { memset(&_obd, 0, sizeof(_obd)); _obd.iFG = OBD_BLACK; _obd.render = 1; _obd.type = OLED_128x64; _obd.iSpeed = 400000;}
    void SPIbegin(int iType = OLED_128x64, int32_t iSpeed = 2000000);
    void setSPIPins(int iCS, int iMOSI, int iSCLK, int iDC, int iReset=-1, int iLED=-1);
    void setI2CPins(int iSDA, int iSCL, int iReset=-1);
    BBI2C *getBB();
    void setBB(BBI2C *pBB);
    OBDISP *getOBD();
    void setFlags(int iFlags);
    int getFlags();
    uint32_t capabilities();
    void setContrast(uint8_t ucContrast);
    int display(bool bRefresh = true, bool bWait = true, bool bFast = false);
    void displayLines(int iStartLine, int iLineCount);
    int dataTime();
    int opTime();
    void setBitBang(bool bBitBang);
    void setRender(bool bRAMOnly);
    void createVirtualDisplay(int width, int height, uint8_t *buffer);
    int I2Cbegin(int iType=OLED_128x64, int iAddr=-1, int32_t iSpeed=400000);
    void setRotation(int iAngle);
    uint8_t getRotation(void);
    void fillScreen(int iColor);
    void setBuffer(uint8_t *pBuffer);
    void backlight(int bOn);
    int allocBuffer(void);
    void * getBuffer(void);
    void freeBuffer(void);
    void setScroll(bool bScroll);
    void setTextSize(uint8_t s);
    void setTextSize(uint8_t sx, uint8_t sy);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawSprite(uint8_t *pSprite, int cx, int cy, int iPitch, int x, int y, uint8_t iPriority);
    void drawTile(const uint8_t *pTile, int x, int y, int iRotation, int bInvert, int bRender);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setTextColor(int iFG, int iBG = -1);
    void setCursor(int x, int y);
    void setPower(bool bOn);
    int loadBMP(const uint8_t *pBMP, int x, int y, int iFG, int iBG);
    int loadG5Image(const uint8_t *pG5, int x, int y, int iFG = OBD_BLACK, int iBG = OBD_WHITE, float fScale = 1.0f);
    int16_t getCursorX(void);
    int16_t getCursorY(void);
    void wake(void);
    void sleep(int bDeep);
    void getStringBox(const char *string, BB_RECT *pRect);
    #ifdef ARDUINO
    void getStringBox(const String &str, BB_RECT *pRect);
    #endif
    void setTextWrap(bool bWrap);
    void setFont(int iFont);
    void setFont(const void *pFont);
    int16_t height(void);
    int16_t width(void);
    void setPosition(int x, int y, int w, int h);
    int scrollBuffer(int iStartCol, int iEndCol, int iStartRow, int iEndRow, int bUp);
    void pushPixels(uint8_t *pPixels, int iCount);
    void writeCommand(uint8_t ucCMD);
    void writeRaw(uint8_t *pData, int iLen);
    void pushImage(int x, int y, int w, int h, uint16_t *pixels);
    void drawString(const char *pText, int x, int y);
#ifdef ARDUINO
    void drawString(String text, int x, int y);
#endif
    void drawLine(int x1, int y1, int x2, int y2, int iColor);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
    void fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
    void drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color);
    void fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color);
    int drawGFX(uint8_t *pSrc, int iSrcCol, int iSrcRow, int iDestCol, int iDestRow, int iWidth, int iHeight, int iSrcPitch);
#ifdef __LINUX__
    void print(const char *pString);
    void println(const char *pString);
    void print(int, int);
    void println(int, int);
    void print(const string &);
    void println(const string &);
    size_t write(uint8_t ucChar);
    void delayMicroseconds(int iTime);
#else
#ifndef __AVR__
    using Print::write;
    virtual size_t write(uint8_t);
#endif // !__AVR__
#endif // __LINUX__

  private:
    OBDISP _obd;
}; // class BB_SPI_LCD
#endif // __cplusplus

typedef char * (*SIMPLECALLBACK)(int iMenuItem);

typedef struct smenu {
  uint8_t u8Up, u8Dn, u8Enter; // button pin numbers
  uint8_t bIsRotary; // rotary encoder or up/down buttons?
  uint8_t bCenter; // center all menu text if true
  uint8_t u8BtnState; // state of all buttons
  uint8_t bOneButton; // flag indicating the menu operates from a single button
  uint8_t prevNextCode; // rotary encoder state machine
  int iMenuIndex; // current menu index
  int iMenuLen; // number of entries in the menu (calculated at startup)
  char **pMenuText; // string array with menu title and text
  int iFontSize;
  int iPressed; // polarity of button pressed state
  unsigned long ulPressTime; // time in millis when button was pressed
  int iDispX, iDispY; // display width/height in pixels
  SIMPLECALLBACK pfnCallback;
  OBDISP *pOBD; // display structureme
  uint16_t store;
} SIMPLEMENU;

// Make the Linux library interface C instead of C++
#if defined(__LINUX__) && defined(__cplusplus)
extern "C" {
#endif

#if !defined(BITBANK_LCD_MODES)
#define BITBANK_LCD_MODES
typedef enum
{
 MODE_DATA = 0,
 MODE_COMMAND
} DC_MODE;
#endif

typedef enum
{
  COM_I2C = 0,
  COM_SPI,
} COM_MODE;

typedef enum
{
  ROT_0 = 0,
  ROT_90,
  ROT_180,
  ROT_270
} FONT_ROTATION;

// These are defined the same in my SPI_LCD library
#ifndef SPI_LCD_H

// Bytewise commands for rendering scenes
// stored in the lower 4 bits of the command byte
// the upper 4 bits can hold single bit parameters
#define OBD_FILL     0
#define OBD_DRAWTEXT 1
#define OBD_DRAWLINE 2
#define OBD_DRAWRECT 3
#define OBD_DRAWELLIPSE 4
#define OBD_DRAWSPRITE  5

// Return value from obd obdI2CInit()
enum {
  OLED_NOT_FOUND = -1, // no display found
  OLED_SSD1306_3C,  // SSD1306 found at 0x3C
  OLED_SSD1306_3D,  // SSD1306 found at 0x3D
  OLED_SH1106_3C,   // SH1106 found at 0x3C
  OLED_SH1106_3D,   // SH1106 found at 0x3D
  OLED_SH1107_3C,  // SH1107
  OLED_SH1107_3D,
  LCD_OK,
  LCD_ERROR
};

//
// Create a virtual display of any size
// The memory buffer must be provided at the time of creation
//
void obdCreateVirtualDisplay(OBDISP *pOBD, int width, int height, uint8_t *buffer);
// Constants for the obdCopy() function
// Output format options -
#define OBD_LSB_FIRST     0x001
#define OBD_MSB_FIRST     0x002
#define OBD_VERT_BYTES    0x004
#define OBD_HORZ_BYTES    0x008
// Orientation options -
#define OBD_ROTATE_90     0x010
#define OBD_FLIP_VERT     0x020
#define OBD_FLIP_HORZ     0x040
#define OBD_INVERT        0x080
// Copy the current bitmap buffer from its native form (LSB_FIRST, VERTICAL_BYTES) to the requested form
// A copy of the same format will just do a memcpy
int obdCopy(OBDISP *pOBD, int iFlags, uint8_t *pDestination);
//
// Draw the contents of a memory buffer onto a display
// The sub-window will be clipped if it specifies too large an area
// for the destination display. The source OBDISP structure must have
// a valid back buffer defined
// The top and bottom destination edges will be drawn on byte boundaries (8 rows)
// The source top/bot edges can be on pixel boundaries
//
void obdDumpWindow(OBDISP *pOBDSrc, OBDISP *pOBDDest, int srcx, int srcy, int destx, int desty, int width, int height);
//
// Write one of more lines to a Sharp memory LCD
//
void obdWriteLCDLines(OBDISP *pOBD, int iStart, int iCount);
//
// Initializes the display controller into "page mode" on I2C
// If SDAPin and SCLPin are not -1, then bit bang I2C on those pins
// Otherwise use the Wire library.
// If you don't need to use a separate reset pin, set it to -1
//
int obdI2CInit(OBDISP *pOBD, int iType, int iAddr, int bFlip, int bInvert, int bWire, int iSDAPin, int iSCLPin, int iResetPin, int32_t iSpeed);
//
// Initialize an SPI version of the display
//
void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int iBitBang, int32_t iSpeed);
//
// Set the drawing direction in 90 degree increments
//
void obdSetRotation(OBDISP *pOBD, int iRotation);
//
// Set the memory configuration to display the pixels at 0 or 180 degrees (flipped)
// pass true (1) to flip 180, false (0) to set to 0
void obdSetFlip(OBDISP *pOBD, int iOnOff);
//
// Provide or revoke a back buffer for your OLED graphics
// This allows you to manage the RAM used by ss_oled on tiny
// embedded platforms like the ATmega series
// Pass NULL to revoke the buffer. Make sure you provide a buffer
// large enough for your display (e.g. 128x64 needs 1K - 1024 bytes)
//
void obdSetBackBuffer(OBDISP *pOBD, uint8_t *pBuffer);
void obdAllocBuffer(OBDISP *pOBD);
//
// Sets the brightness (0=off, 255=brightest)
//
void obdSetContrast(OBDISP *pOBD, unsigned char ucContrast);
//
// Load a 1-bpp Windows bitmap
// Pass the pointer to the beginning of the BMP file
// First pass version assumes a full screen bitmap
//
int obdLoadBMP(OBDISP *pOBD, const uint8_t *pBMP, int x, int y, int iFG, int iBG);
//
// load a 4-bpp Windows bitmap
// into memory for 3-color (BLACK/WHITE/RED)
// e-paper displays
// It does a 'best match' of the colors to
// B/W/R
//
int obdLoadBMP3(OBDISP *pOBD, const uint8_t *pBMP, int dx, int dy);
//
// Power up/down the display
// useful for low power situations
//
void obdPower(OBDISP *pOBD, int bOn);
//
// Set the current cursor position
// The column represents the pixel column (0-127)
// The row represents the text row (0-7)
//
void obdSetCursor(OBDISP *pOBD, int x, int y);

//
// Turn text wrap on or off for the obdWriteString() function
//
void obdSetTextWrap(OBDISP *pOBD, int bWrap);
//
// Advance to the next line
//
void obdNextLine(OBDISP *pOBD);
//
// Draw a string of normal (8x8), small (6x8) or large (16x32) characters
// At the given col+row with the given scroll offset. The scroll offset allows you to
// horizontally scroll text which does not fit on the width of the display. The offset
// represents the pixels to skip when drawing the text. An offset of 0 starts at the beginning
// of the text.
// The system remembers where the last text was written (the cursor position)
// To continue writing from the last position, set the x,y values to -1
// The text can optionally wrap around to the next line by calling oledSetTextWrap(true);
// otherwise text which would go off the right edge will not be drawn and the cursor will
// be left "off screen" until set to a new position explicitly
//
//  Returns 0 for success, -1 for invalid parameter
//
int obdWriteString(OBDISP *pOBD, int iScrollX, int x, int y, char *szMsg, int iSize, int iColor, int bRender);
//
// Draw a string with a fractional scale in both dimensions
// the scale is a 16-bit integer with and 8-bit fraction and 8-bit mantissa
// To draw at 1x scale, set the scale factor to 256. To draw at 2x, use 512
// The output must be drawn into a memory buffer, not directly to the display
// The string can be drawn in one of 4 rotations (ROT_0, ROT_90, ROT_180, ROT_270)
//
int obdScaledString(OBDISP *pOBD, int x, int y, char *szMsg, int iSize, int iColor, int iXScale, int iYScale, int iRotation);
//
// Draw a string in a proportional font you supply
// Requires a back buffer
//
int obdWriteStringCustom(OBDISP *pOBD, void *pFont, int x, int y, char *szMsg, uint8_t ucColor);
//
// Get the width of text in a custom font
//
void obdGetStringBox(void *pFont, char *szMsg, int *width, int *top, int *bottom);
//
// Fill the frame buffer with a byte pattern
// e.g. all off (0x00) or all on (0xff)
//
void obdFill(OBDISP *pOBD, unsigned char ucData, int bRender);
//
// Set (or clear) an individual pixel
// The local copy of the frame buffer is used to avoid
// reading data from the display controller
// (which isn't possible in most configurations)
// This function needs the USE_BACKBUFFER macro to be defined
// otherwise, new pixels will erase old pixels within the same byte
//
int obdSetPixel(OBDISP *pOBD, int x, int y, unsigned char ucColor, int bRender);
//
// Dump a portion of the screen
// optional buffer pointer if no back buffer
//
int obdDumpPartial(OBDISP *pOBD, int startx, int starty, int width, int height, uint8_t *pBuffer);
//
// Dump an entire custom buffer to the display
// useful for custom animation effects
//
int obdDumpBuffer(OBDISP *pOBD, uint8_t *pBuffer, int bRefresh, int bWait, int bFast);
//
// Render a window of pixels from a provided buffer or the library's internal buffer
// to the display. The row values refer to byte rows, not pixel rows due to the memory
// layout of OLEDs. Pass a src pointer of NULL to use the internal backing buffer
// returns 0 for success, -1 for invalid parameter
//
int obdDrawGFX(OBDISP *pOBD, uint8_t *pSrc, int iSrcCol, int iSrcRow, int iDestCol, int iDestRow, int iWidth, int iHeight, int iSrcPitch);
//
// Set the current custom font pointers for playing back
// bytewise commands
//
void obdSetCustomFont(OBDISP *pOBD, void *pFont, uint8_t ucFont);
//
// Execute a set of bytewise command bytes
// and execute the drawing instructions on the current display/buffer
// Optionally render on backbuffer or physical display
//
void obdExecCommands(uint8_t *pData, int iLen, OBDISP *pOBD, int bRender);
//
// Return the number of bytes accumulated as commands
//
int obdGetCommandLen(OBDISP *pOBD);
//
// Draw a line between 2 points
//
void obdDrawLine(OBDISP *pOBD, int x1, int y1, int x2, int y2, uint8_t ucColor, int bRender);
//
// Play a frame of animation data
// The animation data is assumed to be encoded for a full frame of the display
// Given the pointer to the start of the compressed data,
// it returns the pointer to the start of the next frame
// Frame rate control is up to the calling program to manage
// When it finishes the last frame, it will start again from the beginning
//
uint8_t * obdPlayAnimFrame(OBDISP *pOBD, uint8_t *pAnimation, uint8_t *pCurrent, int iLen);
void obdWriteCommand(OBDISP *pOBD, unsigned char c);
void obdSetPosition(OBDISP *pOBD, int x, int y, int bRender);
void obdWriteDataBlock(OBDISP *pOBD, unsigned char *ucBuf, int iLen, int bRender);
void RawWriteData(OBDISP *pOBD, unsigned char *ucBuf, int iLen);
//
// Scroll the internal buffer by 1 scanline (up/down)
// width is in pixels, lines is group of 8 rows
// Returns 0 for success, -1 for invalid parameter
//
int obdScrollBuffer(OBDISP *pOBD, int iStartCol, int iEndCol, int iStartRow, int iEndRow, int bUp);
//
// Draw a sprite of any size in any position
// If it goes beyond the left/right or top/bottom edges
// it's trimmed to show the valid parts
// This function requires a back buffer to be defined
// The priority color (0 or 1) determines which color is painted 
// when a 1 is encountered in the source image.
// e.g. when 0, the input bitmap acts like a mask to clear
// the destination where bits are set.
//
void obdDrawSprite(OBDISP *pOBD, uint8_t *pSprite, int cx, int cy, int iPitch, int x, int y, uint8_t iPriority);

//
// Draw a 16x16 tile in any of 4 rotated positions
// Assumes input image is laid out like "normal" graphics with
// the MSB on the left and 2 bytes per line
// On AVR, the source image is assumed to be in FLASH memory
// The function can draw the tile on byte boundaries, so the x value
// can be from 0 to 112 and y can be from 0 to 6
//
void obdDrawTile(OBDISP *pOBD, const uint8_t *pTile, int x, int y, int iRotation, int bInvert, int bRender);
//
// Draw an outline or filled ellipse
//
void obdEllipse(OBDISP *pOBD, int iCenterX, int iCenterY, int32_t iRadiusX, int32_t iRadiusY, uint8_t ucColor, uint8_t bFilled);
//
// Draw an outline or filled rectangle
//
void obdRectangle(OBDISP *pOBD, int x1, int y1, int x2, int y2, uint8_t ucColor, uint8_t bFilled);

//
// Turn the LCD backlight on or off
//
void obdBacklight(OBDISP *pODB, int bOn);

//
// Menu functions
//
// Initialize the simple menu structure
//
int obdMenuInit(OBDISP *pOBD, SIMPLEMENU *sm, char **pText, int iFontSize, int bCenter, int btnUp, int btnDn, int btnEnter, int iPressedState, int bIsRotary);
//
// Erase the display and show the given menu
//
void obdMenuShow(SIMPLEMENU *sm, int iItem);
//
// Set a callback function to return custom info/status
// for each menu item
//
void obdMenuSetCallback(SIMPLEMENU *sm, SIMPLECALLBACK pfnCallBack);
//
// Display the text of a single menu item
// optionally erases what's under it to prevent left-over text when the length changes
//
void obdMenuShowItem(OBDISP *pOBD, int x, int y, char *szText, int bInvert, int bErase, int iFontSize, int bRender);
//
// Change the menu index incrementally
// redraws the minimum amount of screen to show the new info
// (this prevents flicker/flash and saves battery life)
// returns the new menu index
//
int obdMenuDelta(SIMPLEMENU *sm, int iDelta);
//
// With the given setup, check for button presses
// and act accordingly
// returns -1 for normal interactions and the menu item index if the user presses the ENTER button
//
int obdMenuRun(SIMPLEMENU *sm);

#if defined(__LINUX__) && defined(__cplusplus)
}
#endif // __LINUX__

#endif // __ONEBITDISPLAY__

