#ifndef __ONEBITDISPLAY__
#define __ONEBITDISPLAY__

#include <BitBang_I2C.h>

// Proportional font data taken from Adafruit_GFX library
/// Font data stored PER GLYPH
#ifndef _ADAFRUIT_GFX_H
typedef struct {
  uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
  uint8_t width;         ///< Bitmap dimensions in pixels
  uint8_t height;        ///< Bitmap dimensions in pixels
  uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
  int8_t xOffset;        ///< X dist from cursor pos to UL corner
  int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
  uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
  GFXglyph *glyph;  ///< Glyph array
  uint8_t first;    ///< ASCII extents (first char)
  uint8_t last;     ///< ASCII extents (last char)
  uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;
#endif // _ADAFRUIT_GFX_H

typedef struct obdstruct
{
uint8_t oled_addr; // requested address or 0xff for automatic detection
uint8_t wrap, flip, type;
uint8_t *ucScreen;
int iCursorX, iCursorY;
int width, height;
int iScreenOffset;
BBI2C bbi2c;
uint8_t com_mode; // communication mode (I2C / SPI)
uint8_t mode; // data/command mode for 9-bit SPI
uint8_t iDCPin, iMOSIPin, iCLKPin, iCSPin;
uint8_t iLEDPin; // backlight
uint8_t bBitBang;
} OBDISP;

typedef char * (*SIMPLECALLBACK)(int iMenuItem);

typedef struct smenu {
  uint8_t u8Up, u8Dn, u8Enter; // button pin numbers
  uint8_t bIsRotary; // rotary encoder or up/down buttons?
  uint8_t bCenter; // center all menu text if true
  uint8_t u8BtnState; // state of all buttons
  uint8_t bOneButton; // flag indicating the menu operates from a single button
  int iMenuIndex; // current menu index
  int iMenuLen; // number of entries in the menu (calculated at startup)
  char **pMenuText; // string array with menu title and text
  int iFontSize;
  int iPressed; // polarity of button pressed state
  unsigned long ulPressTime; // time in millis when button was pressed
  int iDispX, iDispY; // display width/height in pixels
  uint8_t prevNextCode; // rotary encoder state machine
  uint16_t store;
  SIMPLECALLBACK pfnCallback;
  OBDISP *pOBD; // display structureme
} SIMPLEMENU;

// Make the Linux library interface C instead of C++
#if defined(_LINUX_) && defined(__cplusplus)
extern "C" {
#endif

typedef enum
{
 MODE_DATA = 0,
 MODE_COMMAND
} DC_MODE;

typedef enum
{
  COM_I2C = 0,
  COM_SPI,
  COM_BLE,
  COM_UART
} COM_MODE;

// These are defined the same in my SPI_LCD library
#ifndef SPI_LCD_H

// 4 possible font sizes: 8x8, 16x32, 6x8, 16x16 (stretched from 8x8)
enum {
   FONT_NORMAL = 0,
   FONT_LARGE,
   FONT_SMALL,
   FONT_STRETCHED
};
#endif

// Display type for init function
enum {
  OLED_128x128 = 1,
  OLED_128x32,
  OLED_128x64,
  OLED_132x64,
  OLED_64x32,
  OLED_96x16,
  OLED_72x40,
  LCD_UC1701,
  LCD_HX1230,
  LCD_NOKIA5110,
  LCD_VIRTUAL
};

// Rotation and flip angles to draw tiles
enum {
  ANGLE_0=0,
  ANGLE_90,
  ANGLE_180,
  ANGLE_270,
  ANGLE_FLIPX,
  ANGLE_FLIPY
};

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
// Initializes a virtual display over BLE
// Currently only OLED_128x64 is supported
// 
int obdBLEInit(OBDISP *pOBD, int iType, int bFlip, int bInvert, char *name);
//
// Initializes a virtual display over BLE
// Currently only OLED_128x64 is supported
//
int obdUARTInit(OBDISP *pOBD, int iType, int bFlip, int bInvert, unsigned long ulSpeed);

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
// Provide or revoke a back buffer for your OLED graphics
// This allows you to manage the RAM used by ss_oled on tiny
// embedded platforms like the ATmega series
// Pass NULL to revoke the buffer. Make sure you provide a buffer
// large enough for your display (e.g. 128x64 needs 1K - 1024 bytes)
//
void obdSetBackBuffer(OBDISP *pOBD, uint8_t *pBuffer);
//
// Sets the brightness (0=off, 255=brightest)
//
void obdSetContrast(OBDISP *pOBD, unsigned char ucContrast);
//
// Load a 1-bpp Windows bitmap
// Pass the pointer to the beginning of the BMP file
// First pass version assumes a full screen bitmap
//
int obdLoadBMP(OBDISP *pOBD, uint8_t *pBMP, int x, int y, int bInvert);
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
int obdWriteString(OBDISP *pOBD, int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender);
//
// Draw a string in a proportional font you supply
// Requires a back buffer
//
int obdWriteStringCustom(OBDISP *pOBD, GFXfont *pFont, int x, int y, char *szMsg, uint8_t ucColor);
//
// Get the width of text in a custom font
//
void obdGetStringBox(GFXfont *pFont, char *szMsg, int *width, int *top, int *bottom);
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
// Dump an entire custom buffer to the display
// useful for custom animation effects
//
void obdDumpBuffer(OBDISP *pOBD, uint8_t *pBuffer);
//
// Render a window of pixels from a provided buffer or the library's internal buffer
// to the display. The row values refer to byte rows, not pixel rows due to the memory
// layout of OLEDs. Pass a src pointer of NULL to use the internal backing buffer
// returns 0 for success, -1 for invalid parameter
//
int obdDrawGFX(OBDISP *pOBD, uint8_t *pSrc, int iSrcCol, int iSrcRow, int iDestCol, int iDestRow, int iWidth, int iHeight, int iSrcPitch);

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

#if defined(_LINUX_) && defined(__cplusplus)
}
#endif // _LINUX_

#endif // __ONEBITDISPLAY__

