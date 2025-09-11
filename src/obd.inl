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
// obd.inl
// display interfacing/control code for OneBitDisplay library
//
#if defined( __AVR__ ) || defined( ARDUINO_ARCH_AVR )
#define WIMPY_MCU
#endif

#if defined(_LINUX_) || defined(ARDUINO_ARCH_MCS51)
#define memcpy_P memcpy
#endif
void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS);
void _delay(int iDelay);
void obdSetDCMode(OBDISP *pOBD, int iMode);
void SPI_BitBang(OBDISP *pOBD, uint8_t *pData, int iLen, uint8_t iMOSIPin, uint8_t iSCKPin);
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen);
void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen);

#ifndef WIMPY_MCU
const uint8_t st7305_init[] PROGMEM = {
    3, 0xd7, 0x17, 0x02, // NVM load ctrl
    2, 0xd1, 0x01, // booster enable
    3, 0xc0, 0x0e, 0x02, // gate voltage
    5, 0xc1, 0x23, 0x23, 0x23, 0x23, // VSHP setting (4.4V)
    5, 0xc2, 0x1e, 0x1e, 0x1e, 0x1e, // VSLP setting (0.6V)
    5, 0xc4, 0x3c, 0x3c, 0x3c, 0x3c, // VSHN setting (-3.7V)
    5, 0xc5, 0x14, 0x14, 0x14, 0x14, // VSLN setting (0.6V)
    3, 0xd8, /*0x80, 0xe9,*/ 0xa6, 0xe9, // OSC setting
    2, 0xb2, 0x02, // frame rate (HPM=16Hz, LPM=1Hz)
    11, 0xb3, 0xe5, 0xf6, 0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45, // HPM EQ Control
    9, 0xb4, 0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45, // LPM EQ ctrl
    4, 0x62, 0x32, 0x03, 0x1f, // gate timing ctrl
    2, 0xb7, 0x13, // source EQ enable
    2, 0xb0, 0x60, // 384 lines
    1, 0x11, // sleep out
    1, 0xff, 200, // 200ms delay
    2, 0xc9, 0x00, // source voltage select
    2, 0x36, 0x00, // memory data access control
    2, 0x3a, 0x11, // data format select (11 = 3 writes for 24bits)
    2, 0xb9, 0x20, // gamma mode (23: mono, 03: 4 grayscale)
    2, 0xb8, 0x29, // panel setting
    3, 0x2a, 0x17, 0x24, // column address setting
    3, 0x2b, 0x00, 0xbf, // row address setting
    2, 0x35, 0x00, // TE
    2, 0xd0, 0xff, // auto power down
    1, 0x39, // 0x39=LPM, 0x38=HPM
    1, 0x29, // display on
    0
};
const uint8_t st7302_wenting[] PROGMEM = {
    0x02, 0xEB, 0x02, // Enable OTP
    0x02, 0xD7, 0x68, // OTP Load Control
    0x02, 0xD1, 0x01, // Auto Power Control
    0x02, 0xC0, 0x40, // Gate Voltage setting
    0x07, 0xC1, 0x22, 0x22, 0x22, 0x22, 0x14, 0x00, // VSH Setting
    0x05, 0xC2, 0x00, 0x00, 0x00, 0x00, // VSL Setting
    0x02, 0xCB, 0x0E, // VCOMH Setting
    0x0B, 0xB4, 0xE5, 0x66, 0x85, 0xFF, 0xFF, 0x52, 0x85, 0xFF, 0xFF, 0x52, // Gate EQ
    0x03, 0xC7, 0xA6, 0xE9, // OSC Setting
    0x02, 0xB0, 0x3F, // Duty Setting
    0x03, 0xB2, 0x00, 0x00, // Frame Rate Setting (lowest for both HPM/LPM)
    0x02, 0x36, 0x00, // Memory Access Mode
    0x02, 0x3A, 0x11, // Data Format
    0x02, 0xB9, 0x23, // Source Setting
    0x02, 0xB8, 0x09, // Source Setting
    0x01, 0x11, // Sleep out
    0x02, 0xff, 100, // delay 100
    0x02, 0xD0, 0x1F, // enable auto power down
    0x01, 0x39, // Low power mode
    0x01, 0x29, // display on
    0x00
};

const uint8_t st7302_hpm_init[] PROGMEM = {
    0x02, 0xEB, 0x02, // Enable OTP
    0x02, 0xD7, 0x68, // OTP Load Control
    0x02, 0xD1, 0x01, // Auto Power Control
    0x02, 0xC0, 0xba , // Gate Voltage setting
    0x02, 0xCB, 0x00, // VCOMH Setting
    0x02, 0xB3, 0x94, // VCOMEQ enable
    0x07, 0xC1, 0x28, 0x28, 0x28, 0x28, 0x14, 0x00, // source voltage VSH
    0x05, 0xC2, 0x00, 0x00, 0x00, 0x00, // VSL Setting
    0x03, 0xB2, 0x01, 0x05, // Frame Rate Setting (lowest for both HPM/LPM)
    0x0B, 0xB4, 0xA5, 0x66, 0xFD, 0xBF, 0x42, 0x55, 0x81, 0xBE, 0x42, 0x55, // Gate EQ
    0x02, 0xB7, 0x01, // Source eq enable
    0x01, 0x11, // Sleep out
    0x02, 0xff, 100, // delay 100
    0x02, 0x72, 0x13, // gate setting
    0x02, 0xB0, 0x3F, // Duty Setting
    0x03, 0xC7, 0xA6, 0xE9, // OSC Setting
    0x02, 0xD6, 0x00, // VSHLSEL source voltage select
    0x02, 0x36, 0x00, // Memory Access Mode
    0x02, 0x3A, 0x11, // Data Format
    0x02, 0xB9, 0x23, // Source Setting
    0x02, 0xB8, 0x09, // Source Setting
    0x02, 0x35, 0x00, // tearing effect line on
//    0x02, 0xD0, 0x1F, // enable auto power down
    0x01, 0x38, // High power mode
    0x01, 0x29, // display on
    0x00
};
#endif // !WIMPY_MCU
// Initialization sequences
const unsigned char oled128_initbuf[] PROGMEM = {0x00, 0xae,0xdc,0x00,0x81,0x40,
      0xa1,0xc8,0xa8,0x7f,0xd5,0x50,0xd9,0x22,0xdb,0x35,0xb0,0xda,0x12,
      0xa4,0xa6,0xaf};

const unsigned char oled64x128_initbuf[] PROGMEM ={
0x00, 0xae, 0xd5, 0x51, 0x20, 0xa8, 0x3f, 0xdc, 0x00, 0xd3, 0x60, 0xad, 0x80, 0xa6, 0xa4, 0xa0, 0xc0, 0x81, 0x40, 0xd9, 0x22, 0xdb, 0x35, 0xaf
};

const unsigned char oled132_initbuf[] PROGMEM = {0x00,0xae,0x02,0x10,0x40,0x81,0xa0,0xc0,0xa6,0xa8,0x3f,0xd3,0x00,0xd5,0x80,0xd9,0xf1,0xda,0x12,0xdb,0x40,0x20,0x02,0xa4,0xa6};

const unsigned char oled64_initbuf[] PROGMEM ={0x00,0xae,0xa8,0x3f,0xd3,0x00,0x40,0xa1,0xc8,
      0xda,0x12,0x81,0xff,0xa4,0xa6,0xd5,0x80,0x8d,0x14,
      0xaf,0x20,0x02};

const unsigned char oled32_initbuf[] PROGMEM  = {
0x00,0xae,0xd5,0x80,0xa8,0x1f,0xd3,0x00,0x40,0x8d,0x14,0xa1,0xc8,0xda,0x02,
0x81,0x7f,0xd9,0xf1,0xdb,0x40,0xa4,0xa6,0xaf};

const unsigned char oled80_initbuf[] PROGMEM = {0x00,0xae,0xa1,0xc8,0xaf
};

const unsigned char oled72_initbuf[] PROGMEM ={0x00,0xae,0xa8,0x3f,0xd3,0x00,0x40,0xa1,0xc8,
      0xda,0x12,0x81,0xff,0xad,0x30,0xd9,0xf1,0xa4,0xa6,0xd5,0x80,0x8d,0x14,
      0xaf,0x20,0x02};

// 128x64 ST7567 LCD in I2C mode (addr 0x3f)
const unsigned char lcd128x64_initbuf[] PROGMEM = {0x00,0xa2,0xa0,0xc8,0x25,0x81,0x20,0x2c,0x2e,0x2f,0xaf};

const unsigned char st7567_initbuf[] PROGMEM  = {0xa2, // 1/9 bias @ 1/65 duty
    0x40, // set start line to 0
    0xa1, // normal segment direction
    0xa6, // not inverse display
    0xa4, // not all pixel on (normal display)
    0xc0, // flipped com direction
    0x2f, // power control (booster/regulator/follower all on)
    0x23, // regulation ratio = 3 ?
    0xf8, 0x00, // set booster level x4 (1=x5)
    0x81, 0x3c, // set EV (electronic volume = contrast)
    0xaf // display on
};

const unsigned char uc1701_initbuf[] PROGMEM  = {0xe2, 0x40, 0xa0, 0xc8, 0xa2, 0x2c, 0x2e, 0x2f, 0xf8, 0x00, 0x23, 0x81, 0x28, 0xac, 0x00, 0xa6};

const unsigned char hx1230_initbuf[] PROGMEM  = {0x2f, 0x90, 0xa6, 0xa4, 0xaf, 0x40, 0xb0, 0x10, 0x00};
const unsigned char nokia5110_initbuf[] PROGMEM  = {0x21, 0xa4, 0xb1, 0x04,0x14,0x20,0x0c};

static void obdCachedFlush(OBDISP *pOBD, int bRender);
static void obdCachedWrite(OBDISP *pOBD, uint8_t *pData, uint8_t u8Len, int bRender);
void obdSetPosition(OBDISP *pOBD, int x, int y, int bRender);
void obdWriteCommand(OBDISP *pOBD, unsigned char c);
void obdWriteDataBlock(OBDISP *pOBD, unsigned char *ucBuf, int iLen, int bRender);
const uint8_t ucMirror[256] PROGMEM =
     {0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
      8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
      4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
      12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
      2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
      10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
      6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
      14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
      1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
      9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
      5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
      13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
      3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
      11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
      7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
      15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255};

//
// Draw the contents of a memory buffer onto a display
// The sub-window will be clipped if it specifies too large an area
// for the destination display. The source OBDISP structure must have
// a valid back buffer defined
// The top and bottom destination edges will be drawn on byte boundaries (8 rows)
// The source top/bot edges can be on pixel boundaries
// This can be used for partial screen updates
//
void obdDumpWindow(OBDISP *pOBDSrc, OBDISP *pOBDDest, int srcx, int srcy, int destx, int desty, int width, int height)
{
uint8_t *s,ucTemp[32]; // temp buffer to gather source pixels
int x, y, tx, i;
int iPitch;

   if (pOBDSrc == NULL || pOBDDest == NULL || pOBDSrc->ucScreen == NULL)
      return; // invalid pointers
   if (width > pOBDDest->width)
      width = pOBDDest->width;
   if (height > pOBDDest->height)
      height = pOBDDest->height;
   iPitch = pOBDSrc->width;
   for (y=0; y<height; y+=8)
   {
      obdSetPosition(pOBDDest, destx, (desty+y)/8, 1);
      for (x=0; x<width; x+=32)
      {
         tx = 32;
         if (width-x < 32) tx = width-x;
         s = &pOBDSrc->ucScreen[((srcy+y)/8)*iPitch + srcx+x];
     if (srcy & 7) // need to shift the bits to get 8 rows of src data
         {
            uint8_t uc, ucShift = srcy & 7;
            for (i=0; i<tx; i++)
            { // combine current and next line to capture 8 pixels
               uc = s[0] >> ucShift;
               uc |= s[iPitch] << (7-ucShift);
               ucTemp[i] = uc;
            }
            obdCachedWrite(pOBDDest, ucTemp, tx, 1);
         }
         else
         { // simpler case
            obdCachedWrite(pOBDDest, s, tx, 1); // just copy it
         }
      } // for x
   } // for y
   obdCachedFlush(pOBDDest, 1);
} /* obdDumpWindow() */

//
// Write a subset of lines to a Sharp memory LCD
//
void obdWriteLCDLines(OBDISP *pOBD, int iStart, int iCount)
{
    int x, y, j;
    uint8_t uc, c, ucMask, *s, *d, ucStart;
    uint8_t ucLineBuf[56]; // 400 pixels is max supported width = 50 bytes + 4
    int iPitch = pOBD->width / 8;
    static int iVCOM = 0;

    if (pOBD == NULL || pOBD->chip_type != OBD_CHIP_SHARP || pOBD->ucScreen == NULL || iStart < 0 || iStart > pOBD->native_height-1 || iStart+iCount < 0 || iStart+iCount > pOBD->native_height-1) {
        return; // invalid request
    }
    
      digitalWrite(pOBD->iCSPin, HIGH); // active high

      ucStart = 0x80; // write command
      iVCOM++;
      if (iVCOM & 0x40) // flip it every 64 lines
        ucStart |= 0x40; // VCOM bit
      ucLineBuf[0] = ucStart;
      RawWriteData(pOBD, ucLineBuf, 1); // write command(01) + vcom(02)

     for (y=iStart; y<iStart+iCount; y++) {
     	d = &ucLineBuf[1];
        ucMask = 1 << (y & 7);
        s = &pOBD->ucScreen[(y >> 3) * pOBD->native_width];
     	ucLineBuf[0] = pgm_read_byte(&ucMirror[y+1]); // current line number
     	for (x=0; x<iPitch; x++) {
           uc = 0xff;
           for (j=7; j>=0; j--) {
               c = *s++;
               if (c & ucMask) uc ^= (1 << j);
           }
           *d++ = uc;
        } // for x
        // write this line to the display
        ucLineBuf[iPitch+1] = 0; // end of line
        RawWriteData(pOBD, ucLineBuf, iPitch+2);
     } // for y
    ucLineBuf[0] = 0;
    RawWriteData(pOBD, ucLineBuf, 1); // final transfer
    digitalWrite(pOBD->iCSPin, LOW); // de-activate
} /* obdWriteLCDLines() */

//
// Turn the display on or off
//
void obdPower(OBDISP *pOBD, int bOn)
{
uint8_t ucCMD;
    
    if (!pOBD) return;
    switch (pOBD->type) {
        case LCD_NOKIA5110:
            ucCMD = (bOn) ? 0x20 : 0x24;
            break;
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
        case LCD_ST7302:
        case LCD_ST7305:
            ucCMD = (bOn) ? 0x29 : 0x28;
            break;
#endif
        default: // all other supported displays
            ucCMD = (bOn) ? 0xaf : 0xae;
            break;
        // e-ink panels are put to sleep after each action
    }
  obdWriteCommand(pOBD, ucCMD);
} /* obdPower() */

// Controls the LED backlight
void obdBacklight(OBDISP *pOBD, int bOn)
{
  if (pOBD->iLEDPin != 0xff)
  {
     digitalWrite(pOBD->iLEDPin, (bOn) ? HIGH:LOW);
  }
} /* obdBacklight() */

//
// Send the command sequence to power up the LCDs
static void LCDPowerUp(OBDISP *pOBD)
{
#ifndef MEMORY_ONLY
    int iLen=0;
    uint8_t *s=NULL;
    u8Cache[0] = 0x00; // send everything in command mode
    if (pOBD->type == LCD_UC1701 || pOBD->type == LCD_UC1609)
    {
        s = (uint8_t *)uc1701_initbuf;
        iLen = sizeof(uc1701_initbuf);
    }
    else if (pOBD->type == LCD_HX1230)
    {
        s = (uint8_t *)hx1230_initbuf;
        iLen = sizeof(hx1230_initbuf);
    }
    else if (pOBD->type == LCD_NOKIA5110) // Nokia 5110
    {
        s = (uint8_t *)nokia5110_initbuf;
        iLen = sizeof(nokia5110_initbuf);
    }
    else if (pOBD->type == LCD_32x64)
    {
        s = (uint8_t *)st7567_initbuf;
        iLen = sizeof(st7567_initbuf);
    }
    memcpy_P(&u8Cache[1], s, iLen);
    RawWrite(pOBD, u8Cache, iLen+1);
    _delay(100);
    obdWriteCommand(pOBD, 0xa5);
    _delay(100);
    obdWriteCommand(pOBD, 0xa4);
    obdWriteCommand(pOBD, 0xaf);
#endif // MEMORY_ONLY
} /* LCDPowerUp() */
//
// Initialize the display controller on an SPI bus
//
void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int bBitBang, int32_t iSpeed)
{
#ifndef MEMORY_ONLY
    uint8_t *s=NULL;
    int iLen=0;
    
    if (pOBD == NULL)
        return;
    pOBD->iTimeout = 5000; // 5 seconds for most e-ink panels
    pOBD->can_flip = 1; // assume can flip 180
    pOBD->ucScreen = NULL; // start with no backbuffer; user must provide one later
    pOBD->x_offset = 0; // e-paper memory column offset
    pOBD->iDCPin = iDC;
    pOBD->iCSPin = iCS;
    pOBD->iRSTPin = iReset;
    pOBD->iMOSIPin = iMOSI;
    pOBD->iCLKPin = iCLK;
    pOBD->iLEDPin = iLED;
    pOBD->type = iType;
    pOBD->chip_type = OBD_CHIP_SSD13xx; // assume common OLEDs
    pOBD->flip = bFlip;
    pOBD->invert = bInvert;
    pOBD->bBitBang = bBitBang;
    pOBD->wrap = 0; // default - disable text wrap
    pOBD->com_mode = COM_SPI; // communication mode
    if (pOBD->iDCPin != 0xff) // Note - not needed on Sharp Memory LCDs
    {
        pinMode(pOBD->iDCPin, OUTPUT);
        digitalWrite(pOBD->iDCPin, 0); // for some reason, command mode must be set or some OLEDs/LCDs won't initialize correctly even if set later
    }
    if (pOBD->iCSPin != 0xff)
    {
        pinMode(pOBD->iCSPin, OUTPUT);
        if (iType == SHARP_400x240 || iType == SHARP_128x128 || iType == SHARP_160x68) {
            digitalWrite(pOBD->iCSPin, LOW);
        } else {
            digitalWrite(pOBD->iCSPin, HIGH); // set to not-active
        }
    }
    if (bBitBang)
    {
        pinMode(iMOSI, OUTPUT);
        pinMode(iCLK, OUTPUT);
    }
    
    // Reset it
    if (pOBD->iRSTPin != 0xff)
    {
        pinMode(pOBD->iRSTPin, OUTPUT);
        digitalWrite(pOBD->iRSTPin, HIGH);
        _delay(200);
        digitalWrite(pOBD->iRSTPin, LOW);
        _delay(10);
        digitalWrite(pOBD->iRSTPin, HIGH);
        _delay(200);
    }
    if (iLED != -1 && iLED != 0xff) {
            pinMode(iLED, OUTPUT);
    }
    initSPI(pOBD, iSpeed, iMOSI, iCLK, iCS);
    pOBD->native_width = pOBD->width = 128; // assume 128x64
    pOBD->height = 64;
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
    if (iType == LCD_ST7302)
    {
        pOBD->native_width = pOBD->width = 250;
        pOBD->native_height = pOBD->height = 122;
        pOBD->flip = 0;
    }
    if (iType == LCD_ST7305)
    {
        pOBD->native_width = pOBD->width = 384;
        pOBD->native_height = pOBD->height = 168;
        pOBD->flip = 0;
    }
#endif
    if (iType == OLED_80x128)
    {
        pOBD->native_width = pOBD->width = 80;
        pOBD->native_height = pOBD->height = 128;
    }
    else if (iType == SHARP_160x68)
    {     
        pOBD->native_width = pOBD->width = 160;
        pOBD->native_height = pOBD->height = 68;
        pOBD->chip_type = OBD_CHIP_SHARP;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
    } 
    else if (iType == SHARP_128x128)
    {
        pOBD->native_width = pOBD->width = 128;
        pOBD->native_height = pOBD->height = 128;
        pOBD->chip_type = OBD_CHIP_SHARP;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
    }
    else if (iType == SHARP_144x168)
    {
        pOBD->native_width = pOBD->width = 144;
        pOBD->native_height = pOBD->height = 168;
        pOBD->chip_type = OBD_CHIP_SHARP;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
    }
    else if (iType == SHARP_400x240)
    {
        pOBD->native_width = pOBD->width = 400;
        pOBD->native_height = pOBD->height = 240;
        pOBD->chip_type = OBD_CHIP_SHARP;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
        pOBD->iOrientation = 0;
        return;
    }
  else if (iType == LCD_32x64)
  {
      pOBD->native_width = pOBD->width = 32;
      pOBD->native_height = pOBD->height = 64;
  }
  else if (iType == LCD_UC1609)
  {
      pOBD->native_width = pOBD->width = 192;
      pOBD->native_height = pOBD->height = 64;
  }
  else if (iType == LCD_HX1230)
  {
      pOBD->native_width = pOBD->width = 96;
      pOBD->native_height = pOBD->height = 68;
      pOBD->iDCPin = 0xff; // flag this as being 3-wire SPI
  }
  else if (iType == LCD_NOKIA5110)
  {
      pOBD->native_width = pOBD->width = 84;
      pOBD->native_height = pOBD->height = 48;
  }
  else if (iType == OLED_96x16)
  {
      pOBD->native_width = pOBD->width = 96;
      pOBD->native_height = pOBD->height = 16;
  }
  else if (iType == OLED_64x128)
  {
      // NOTE: 64x128 SH1107 doesn't seem to properly support
      // segment remap, so the init commands are A0/C0 and
      // it can't be flipped 180
      pOBD->can_flip = 0;
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 128;
  }
  else if (iType == OLED_128x32)
      pOBD->native_height = pOBD->height = 32;
  else if (iType == OLED_128x128)
      pOBD->native_height = pOBD->height = 128;
  else if (iType == OLED_64x48)
  {
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 48;
  }
  else if (iType == OLED_64x32)
  {
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 32;
  }
  else if (iType == OLED_72x40)
  {
      pOBD->native_width = pOBD->width = 72;
      pOBD->native_height = pOBD->height = 40;
  }
  if (iType == OLED_80x128)
  {
     s = (uint8_t *)oled80_initbuf;
     iLen = sizeof(oled80_initbuf);
  }
  else if (iType == OLED_128x32 || iType == OLED_96x16)
  {
     s = (uint8_t *)oled32_initbuf;
     iLen = sizeof(oled32_initbuf);
  }
  else if (iType == OLED_64x128)
  {
      s = (uint8_t *)oled64x128_initbuf;
     iLen = sizeof(oled64x128_initbuf);
  }
  else if (iType == OLED_128x128)
  {
     s = (uint8_t *)oled128_initbuf;
     iLen = sizeof(oled128_initbuf);
  }
  else if (iType == OLED_132x64) { // SH1106
     s = (uint8_t *)oled132_initbuf;
     iLen = sizeof(oled132_initbuf);
  }
  else if (iType < LCD_UC1701) // 128x64 and 64x32 oleds
  {
     s = (uint8_t *)oled64_initbuf;
     iLen = sizeof(oled64_initbuf);
  }
    // OLED
  if (iType < LCD_UC1701)
  {
      memcpy_P(u8Cache, s, iLen); // do it from RAM
      RawWrite(pOBD, u8Cache, iLen);
      _delay(100); // on SPI display this delay is needed or the display
      // never sees the "display on" command at the end of the sequence
      obdWriteCommand(pOBD, 0xaf); // turn on display
      if (bInvert)
      {
          u8Cache[0] = 0; // command
          u8Cache[1] = 0xa7; // invert command
          RawWrite(pOBD, u8Cache, 2);
      }
      if (bFlip) // rotate display 180
      {
          u8Cache[0] = 0; // command
          u8Cache[1] = 0xa0;
          RawWrite(pOBD, u8Cache, 2);
          u8Cache[0] = 0;
          u8Cache[1] = 0xc0;
          RawWrite(pOBD, u8Cache, 2);
      }
  } // OLED
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
  if (iType == LCD_ST7302 || iType == LCD_ST7305)
  {
//     uint8_t *s = (uint8_t *)st7302_hpm_init;
      const uint8_t *s = (iType == LCD_ST7302) ? (uint8_t *)st7302_wenting : (uint8_t *)st7305_init; //st7302_lpm_init;
    iLen = 1;

      while (iLen) {
          iLen = pgm_read_byte(s++); // parameter byte count
          if (iLen) {
              if (pgm_read_byte(s) == 0xff) { // delay
                  _delay(pgm_read_byte(&s[1]));
                  s += 2;
              } else {
                  obdWriteCommand(pOBD, pgm_read_byte(s));
                  if (iLen > 1) {
                      memcpy_P(u8Cache, s, iLen);
                      RawWrite(pOBD, u8Cache, iLen);
                  }
                  s += iLen;
              }
          }
      } // while commands to transmit
      if (pOBD->invert)
          obdWriteCommand(pOBD, 0x21); // inversion on
      // Clear RAM
//      obdWriteCommand(pOBD, 0xb9);
//      uc[0] = 0x40;
//      uc[1] = 0xe3;
//      RawWrite(pOBD, uc, 2);
//      _delay(1000);
//      obdWriteCommand(pOBD, 0xb9);
//      uc[0] = 0x40;
//      uc[1] = 0x23;
//      RawWrite(pOBD, uc, 2);
      return;
  } /* ST7302/ST7305 */
#endif // !WIMPY_MCU
  if (iType == LCD_UC1701 || iType == LCD_HX1230 || iType == LCD_32x64)
  {
      uint8_t cCOM = 0xc0;
      LCDPowerUp(pOBD);
      if (iType == LCD_HX1230)
      {
          obdSetContrast(pOBD, 0); // contrast of 0 looks good
          cCOM = 0xc8;
      }
      if (bFlip) // flip horizontal + vertical
      {
         obdWriteCommand(pOBD, 0xa1); // set SEG direction (A1 to flip horizontal)
         obdWriteCommand(pOBD, cCOM); // set COM direction (C0 to flip vert)
      }
      if (bInvert)
      {
         obdWriteCommand(pOBD, 0xa7); // set inverted pixel mode
      }
  }
  if (iType == LCD_UC1609)
  {
      LCDPowerUp(pOBD);
      return;
//      obdWriteCommand(pOBD, 0xe2); // system reset
//      _delay(100);
//      obdWriteCommand(pOBD, 0xa0); // set frame rate to 76fps
//      obdWriteCommand(pOBD, 0xeb); // set BR
//      obdWriteCommand(pOBD, 0x2f); // set Power Control
//      obdWriteCommand(pOBD, 0xc4); // set LCD mapping control
//      obdWriteCommand(pOBD, 0x81); // set PM
//      obdWriteCommand(pOBD, 0x90); // set contrast to 144
//      _delay(100);
//      obdWriteCommand(pOBD, 0xaf); // display enable
//      if (bFlip) // flip horizontal + vertical
//      {
//         obdWriteCommand(pOBD, 0xa1); // set SEG direction (A1 to flip horizontal)
//         obdWriteCommand(pOBD, 0xc2); // set COM direction (C0 to flip vert)
//      }
//      if (bInvert)
//      {
//         obdWriteCommand(pOBD, 0xa7); // set inverted pixel mode
//      }
  } // UC1609
#endif // MEMORY_ONLY
} /* obdSPIInit() */
void obdSetRotation(OBDISP *pOBD, int iRotation)
{
    pOBD->iScreenOffset = 0;
    
    switch (iRotation) {
        default: return;
        case 0:
            obdSetFlip(pOBD, 0);
            pOBD->iOrientation = 0;
            pOBD->width = pOBD->native_width;
            pOBD->height = pOBD->native_height;
            break;
        case 1:
        case 90:
            obdSetFlip(pOBD, 0);
            pOBD->iOrientation = 90;
            pOBD->width = pOBD->native_height;
            pOBD->height = pOBD->native_width;
            break;
        case 2:
        case 180:
            pOBD->iOrientation = 180;
            obdSetFlip(pOBD, 1);
            pOBD->width = pOBD->native_width;
            pOBD->height = pOBD->native_height;
            break;
        case 3:
        case 270:
            pOBD->iOrientation = 270;
            obdSetFlip(pOBD, 1);
            pOBD->width = pOBD->native_height;
            pOBD->height = pOBD->native_width;
            break;
  }
} /* obdSetRotation() */
//
// Set the memory configuration to display the pixels at 0 or 180 degrees (flipped)
// Pass true (1) to flip 180, false (0) to set to 0
//
void obdSetFlip(OBDISP *pOBD, int iOnOff)
{
   if (pOBD == NULL) return;
   if (!pOBD->can_flip) return; // don't try it on some displays
    
   pOBD->flip = iOnOff;
   if (pOBD->type == LCD_UC1701 || pOBD->type == LCD_UC1609 || pOBD->type == LCD_32x64)
   {
      if (iOnOff) // rotate display 180
      {
         obdWriteCommand(pOBD, 0xa1); // SEG direction (A1 to flip horizontal)
         obdWriteCommand(pOBD, 0xc0); // COM direction (C0 to flip vert)
      } else { // non-rotated
         obdWriteCommand(pOBD, 0xa0);
         obdWriteCommand(pOBD, 0xc8);
      }
   } else { // OLEDs
      if (iOnOff)
      {
          obdWriteCommand(pOBD, 0xa0);
          obdWriteCommand(pOBD, 0xc0);
      } else {
          obdWriteCommand(pOBD, 0xa1);
          obdWriteCommand(pOBD, 0xc8);
      }
   }
} /* obdSetFlip() */
//
// Initializes the OLED controller into "page mode"
//
int obdI2CInit(OBDISP *pOBD, int iType, int iAddr, int bFlip, int bInvert, int bWire, int sda, int scl, int reset, int32_t iSpeed)
{
int rc = OLED_NOT_FOUND;
#ifndef MEMORY_ONLY
unsigned char uc[32];
uint8_t u8Len, *s;

  if (pOBD == NULL)
      return -1;
    pOBD->can_flip = 1; // assume can flip 180
    if (iType < 0 || iType >= LCD_COUNT)
        return -1; // invalid display type
  if (sda == scl) { // invalid situation, set to -1
    sda = scl = -1;
  }
  pOBD->type = iType;
  pOBD->flip = bFlip;
  pOBD->invert = bInvert;
  pOBD->wrap = 0; // default - disable text wrap
  pOBD->bbi2c.iSDA = sda;
  pOBD->bbi2c.iSCL = scl;
  pOBD->bbi2c.bWire = bWire;
  pOBD->com_mode = COM_I2C; // communication mode
#ifdef _LINUX_
  pOBD->bbi2c.iBus = sda;
  pOBD->ucScreen = NULL;
#endif
  I2CInit(&pOBD->bbi2c, iSpeed); // on Linux, SDA = bus number, SCL = device address
#ifdef _LINUX_
  pOBD->oled_addr = (uint8_t)scl;
#else
  // Reset it
  if (reset != 0xff && reset != -1)
  {
    pinMode(reset, OUTPUT);
    digitalWrite(reset, HIGH);
    _delay(50);
    digitalWrite(reset, LOW);
    _delay(50);
    digitalWrite(reset, HIGH);
    _delay(10);
  }
  // find the device address if requested
  if (iAddr == -1 || iAddr == 0 || iAddr == 0xff) // find it
  {
    I2CTest(&pOBD->bbi2c, 0x3c);
    if (I2CTest(&pOBD->bbi2c, 0x3c))
       pOBD->oled_addr = 0x3c;
    else if (I2CTest(&pOBD->bbi2c, 0x3d))
       pOBD->oled_addr = 0x3d;
    else
       return rc; // no display found!
  }
  else
  {
    pOBD->oled_addr = iAddr;
    I2CTest(&pOBD->bbi2c, iAddr);
    if (!I2CTest(&pOBD->bbi2c, iAddr))
       return rc; // no display found
  }

#endif
  // Detect the display controller (SSD1306, SH1107 or SH1106)
  uint8_t u = 0;
  I2CReadRegister(&pOBD->bbi2c, pOBD->oled_addr, 0x00, &u, 1); // read the status register
  u &= 0x0f; // mask off power on/off bit
  if ((u == 0x7 || u == 0xf) && pOBD->type == OLED_128x128) // SH1107
  { // A single SSD1306 display returned 7, so only act on it if the
    // user specified that they're working with a 128x128 display
    rc = OLED_SH1107_3C;
    bFlip = !bFlip; // SH1107 seems to have this reversed from the usual direction
  }
  else if (u == 0x8) // SH1106
  {
    rc = OLED_SH1106_3C;
    pOBD->type = OLED_132x64; // needs to be treated a little differently
  }
  else if (u == 3 || u == 6 || u == 7) // 7=128x64(rare),6=128x64 display, 3=smaller
  {
    rc = OLED_SSD1306_3C;
  }
    if (pOBD->oled_addr == 0x3d) {
        rc++; // return the '3D' version of the type
    }
  if (iType == OLED_80x128)
  {
      s = (uint8_t *)oled80_initbuf;
      u8Len = sizeof(oled80_initbuf);
  }
  if (iType == OLED_128x32 || iType == OLED_96x16)
  {
      s = (uint8_t *)oled32_initbuf;
      u8Len = sizeof(oled32_initbuf);
  }
  else if (iType == OLED_128x128)
  {
      s = (uint8_t *)oled128_initbuf;
      u8Len = sizeof(oled128_initbuf);
  }
  else if (iType == OLED_72x40)
  {
      s = (uint8_t *)oled72_initbuf;
      u8Len = sizeof(oled72_initbuf);
  }
  else if (iType == OLED_64x128)
  {
      s = (uint8_t *)oled64x128_initbuf;
      u8Len = sizeof(oled64x128_initbuf);
      pOBD->can_flip = 0; // this display can't flip 180 (don't know why)
  }
  else if (iType == LCD_128x64_I2C)
  {
      obdWriteCommand(pOBD, 0xe2); // Need to be reset after power-on
      _delay(10);
      s = (uint8_t *)lcd128x64_initbuf;
      u8Len = sizeof(lcd128x64_initbuf);
      rc = LCD_OK;
      memcpy_P(uc, s, u8Len);
  }
  else // 132x64, 128x64, 64x48 and 64x32
  {
      s = (uint8_t *)oled64_initbuf;
      u8Len = sizeof(oled64_initbuf);
  }

    memcpy_P(uc, s, u8Len);
  RawWrite(pOBD, uc, u8Len);
  if (bInvert)
  {
    uc[0] = 0; // command
    uc[1] = 0xa7; // invert command
    RawWrite(pOBD,uc, 2);
  }
  if (bFlip) // rotate display 180
  {
    uc[0] = 0; // command
    uc[1] = 0xa0;
    RawWrite(pOBD,uc, 2);
    uc[1] = 0xc0;
    RawWrite(pOBD,uc, 2);
  }
  pOBD->native_width = pOBD->width = 128; // assume 128x64
  pOBD->native_height = pOBD->height = 64;
  if (iType == OLED_96x16)
  {
      pOBD->native_width = pOBD->width = 96;
      pOBD->native_height = pOBD->height = 16;
  }
  else if (iType == OLED_80x128)
  {
      pOBD->native_width = pOBD->width = 80;
      pOBD->native_height = pOBD->height = 128;
  }
  else if (iType == OLED_64x128)
  {
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 128;
  }
  else if (iType == OLED_128x32)
      pOBD->native_height = pOBD->height = 32;
  else if (iType == OLED_128x128)
      pOBD->native_height = pOBD->height = 128;
  else if (iType == OLED_64x32)
  {
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 32;
  }
  else if (iType == OLED_64x48)
  {
      pOBD->native_width = pOBD->width = 64;
      pOBD->native_height = pOBD->height = 48;
  }
  else if (iType == OLED_72x40)
  {
      pOBD->native_width = pOBD->width = 72;
      pOBD->native_height = pOBD->height = 40;
  }
#endif // MEMORY_ONLY
  return rc;
} /* obdI2CInit() */
//
// Sends a command to turn on or off the OLED display
//
void oledPower(OBDISP *pOBD, uint8_t bOn)
{
    if (bOn)
      obdWriteCommand(pOBD, 0xaf); // turn on OLED
    else
      obdWriteCommand(pOBD, 0xae); // turn off OLED
} /* oledPower() */

// Sets the D/C pin to data or command mode
void obdSetDCMode(OBDISP *pOBD, int iMode)
{
  if (pOBD->iDCPin == 0xff) // 9-bit SPI
      pOBD->mode = (uint8_t)iMode;
  else // set the GPIO line
      digitalWrite(pOBD->iDCPin, (iMode == MODE_DATA));
} /* obdSetDCMode() */

static void obdWriteCommand2(OBDISP *pOBD, unsigned char c, unsigned char d)
{
unsigned char buf[4];

    if (pOBD->com_mode == COM_I2C) {// I2C device
        buf[0] = 0x00;
        buf[1] = c;
        buf[2] = d;
        RawWrite(pOBD, buf, 3);
    } else { // must be SPI
        obdWriteCommand(pOBD, c);
        obdWriteCommand(pOBD, d);
    }
} /* obdWriteCommand2() */

//
// Sets the brightness (0=off, 255=brightest)
//
void obdSetContrast(OBDISP *pOBD, unsigned char ucContrast)
{
  if (pOBD->type == LCD_HX1230)
  { // valid values are 0-31, so scale it
      ucContrast >>= 3;
      obdWriteCommand(pOBD, 0x80 + ucContrast);
  }
  else if (pOBD->type == LCD_NOKIA5110)
  {
      // we allow values of 0xb0-0xbf, so shrink the range
      ucContrast >>= 4;
      obdWriteCommand(pOBD, 0x21); // set advanced command mode
      obdWriteCommand(pOBD, 0xb0 | ucContrast);
      obdWriteCommand(pOBD, 0x20); // set simple command mode
  }
  else // OLEDs + UC1701
      obdWriteCommand2(pOBD, 0x81, ucContrast);
} /* obdSetContrast() */

//
// Special case for ST7302/ST7305
//
#ifndef WIMPY_MCU
void obdST7302SetPos(OBDISP *pOBD, int x, int y)
{
uint8_t ucTemp[4];
    
    obdWriteCommand(pOBD, 0x2a); // Column set
    if (pOBD->type == LCD_ST7302) {
        ucTemp[0] = 0x19 + (y/12); // start x (we treat it as y)
        ucTemp[1] = 0x27; // end x
    } else { // ST7305
        ucTemp[0] = 0x17 + (y/12);
        ucTemp[1] = 0x24;
    }
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0x2b); // Row set
    if (pOBD->type == LCD_ST7302) {
        ucTemp[0] = (x/2); // start y (we treat it as x)
        ucTemp[1] = 0x80; // end y
    } else { // ST7305
        ucTemp[0] = (x/2);
        ucTemp[1] = 0xbf;
    }
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0x2c); // memory write
} /* obdST7302SetPos() */
//
// Special case for ST7302/ST7305
//
static int ST7302DumpBuffer(OBDISP *pOBD, uint8_t *pBuffer)
{
uint8_t ucPixels[64];
int x, y, h, iPitch, count;
uint8_t ucMask, uc1, *s, *d;
// bit expansion table to speed up conversion
const uint8_t u8Expand[16] = {
0x00, 0x80, 0x20, 0xa0, 0x08, 0x88, 0xa0, 0xa8,
0x02, 0x82, 0x22, 0xa2, 0x0a, 0x8a, 0x2a, 0xaa 
};

// Shift out the image in pairs of lines
    iPitch = pOBD->width;
    switch (pOBD->iOrientation) {
        case 0:
        case 180:
            h = pOBD->height;
            if (pOBD->type == LCD_ST7302) h += 8;
            for (x = 0; x < pOBD->width; x += 2) { // a pair of columns at a time
                d = ucPixels;
                if (pOBD->iOrientation == 180) {
                    uint8_t u8_0, u8_1;
                    s = &pBuffer[x + ((h-16)>>3)*iPitch];
                    for (y = 0; y < h; y+=8) {
                        u8_0 = ucMirror[s[1]]; // source image bits are mirrored
                        u8_1 = ucMirror[s[0]];
                        uc1 = u8Expand[u8_0 & 0xf];
                        uc1 |= (u8Expand[u8_1 & 0xf] >> 1);
                        *d++ = uc1;
                        uc1 = u8Expand[u8_0 >> 4];
                        uc1 |= (u8Expand[u8_1 >> 4] >> 1);
                        *d++ = uc1;
                        s -= iPitch;
                    } // for y
                } else { // rotation = 0
                    s = &pBuffer[x];
                    for (y=0; y < h; y+=8) {
                        uc1 = u8Expand[s[0] & 0xf];
                        uc1 |= (u8Expand[s[1] & 0xf] >> 1);
                        *d++ = uc1;
                        uc1 = u8Expand[s[0] >> 4];
                        uc1 |= (u8Expand[s[1] >> 4] >> 1);
                        *d++ = uc1;
                        s += iPitch;
                    } // for y
                } // flipped
                obdST7302SetPos(pOBD, (pOBD->flip)? 248-x:x,0);
                RawWriteData(pOBD, ucPixels, (int)(d - ucPixels));
            } // for x
            break;
        case 90:
        case 270: // x/y swapped
            if (pOBD->iOrientation == 270) {
                for (y=pOBD->height-2; y >= 0; y-=2) {
                    ucMask = 1<<(y & 7);
                    s = &pBuffer[(y>>3)*iPitch];
                    count = 0;
                    d = ucPixels;
                    uc1 = 0;
                    for (x=0; x<iPitch; x++) {
                        uc1 <<= 2;
                        if (s[x] & ucMask) uc1 |= 1;
                        if (s[x] & (ucMask << 1)) uc1 |= 2;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                        }
                    } // for x
                    obdST7302SetPos(pOBD, pOBD->height - 2 - y, 0);
                    RawWriteData(pOBD, ucPixels, (int)(d - ucPixels));
                } // for y
            } else { // 90
                for (y=0; y < pOBD->height; y+=2) {
                    ucMask = 1<<(y & 7);
                    s = &pBuffer[(y>>3)*iPitch];
                    count = 0;
                    d = ucPixels;
                    uc1 = 0;
                    for (x=iPitch-1; x>=0; x--) {
                        uc1 <<= 2;
                        if (s[x] & ucMask) uc1 |= 2;
                        if (s[x] & (ucMask << 1)) uc1 |= 1;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                        }
                    } // for x
                    obdST7302SetPos(pOBD, y, 0);
                    RawWriteData(pOBD, ucPixels, (int)(d - ucPixels));
                } // for y
            } // flipped
            break;
    } // switch
    return OBD_SUCCESS;
} /* ST7302DumpBuffer() */
#endif // !WIMPY_MCU
//
// Special case for Sharp Memory LCD
//
static int SharpDumpBuffer(OBDISP *pOBD, uint8_t *pBuffer)
{
int x, y;
uint8_t c, ucInvert, ucStart, ucMask;
uint8_t *s, *d;
uint8_t ucLineBuf[64];
int iPitch = pOBD->native_width / 8;
static int ucVCOM = 0;
int iBit;

  ucInvert = (pOBD->invert) ? 0x00 : 0xff;
  digitalWrite(pOBD->iCSPin, HIGH); // active high
    ucLineBuf[0] = 0; // Tell RawWrite that it's command (doesn't matter for Sharp LCDs)
  ucStart = 0x80; // write command
  if (ucVCOM)
    ucStart |= 0x40; // VCOM bit
  ucLineBuf[1] = ucStart;
  // this code assumes I2C, so the first byte is ignored
  RawWrite(pOBD, ucLineBuf, 2); // write command(01) + vcom(02)
  ucVCOM = !ucVCOM; // need to toggle this each transaction

 // We need to flip and invert the image in code because the Sharp memory LCD
 // controller only has the simplest of commands for data writing
  if (pOBD->iOrientation == 180)
  {
     for (y=0; y<pOBD->height; y++) // we have to write the memory in the wrong direction
     {
         ucMask = 0x80 >> (y & 7);
        s = &pBuffer[pOBD->width - 1 + (pOBD->width * ((pOBD->height - 1 - y) >> 3))]; // point to last line first
        d = &ucLineBuf[2];
        ucLineBuf[1] = pgm_read_byte(&ucMirror[y+1]); // current line number
        for (x=0; x<pOBD->width/8; x++)
        {
           c = ucInvert; // we need to brute-force invert it
            for (iBit=7; iBit>=0; iBit--)
            {
                if (s[0] & ucMask)
                    c ^= (1 << iBit);
                s--;
            }
           *d++ = c;
        } // for y
        // write this line to the display
        ucLineBuf[iPitch+2] = 0; // end of line
        RawWrite(pOBD, ucLineBuf, iPitch+3);
     } // for x
  }
  else if (pOBD->iOrientation == 0) // normal orientation
  {
     for (y=0; y<pOBD->height; y++) // we have to write the memory in the wrong direction
     {
        ucMask = 1 << (y & 7);
        s = &pBuffer[pOBD->width * (y >> 3)]; // point to last line first
        d = &ucLineBuf[2];
        ucLineBuf[1] = pgm_read_byte(&ucMirror[y+1]); // current line number
        for (x=0; x<pOBD->width/8; x++)
        {
            c = ucInvert;
            for (iBit=7; iBit>=0; iBit--)
            {
                if (s[0] & ucMask)
                    c ^= (1 << iBit);
                s++;
            }
           *d++ = c;
        } // for x
        // write this line to the display
        *d++ = 0; // end of line
        RawWrite(pOBD, ucLineBuf, iPitch+3);
     } // for y
  } else if (pOBD->iOrientation == 90) {
     for (x=0; x<pOBD->width; x++) {
	s = &pBuffer[x+((pOBD->height-1)>>3)*pOBD->width];
	d = &ucLineBuf[2];
	ucLineBuf[1] = pgm_read_byte(&ucMirror[x+1]); // line number
        for (y=0; y<pOBD->height; y+=8) { // we have to write the memory in the wrong direction
            *d++ = s[0] ^ ucInvert;
	    s -= pOBD->width;
        } // for y
	ucLineBuf[iPitch+2] = 0; // end of line
	RawWrite(pOBD, ucLineBuf, iPitch+3);
     } // for x
  } else { // must be 270
     for (x=0; x<pOBD->width; x++) {
	 d = &ucLineBuf[2];
	 s = &pBuffer[(pOBD->width-1-x)];
	 ucLineBuf[1] = pgm_read_byte(&ucMirror[x+1]); // line number
         for (y=0; y<pOBD->height; y+=8) {
	     *d++ = pgm_read_byte(&ucMirror[s[0] ^ ucInvert]);
	     s += pOBD->width;
	 } // for y
	 ucLineBuf[iPitch+2] = 0; // end of line
	 RawWrite(pOBD, ucLineBuf, iPitch+3);
     } // for x
  } // 270
  ucLineBuf[1] = 0;
  RawWrite(pOBD, ucLineBuf, 2); // final transfer
  digitalWrite(pOBD->iCSPin, LOW); // de-activate
    return OBD_SUCCESS;
} /* SharpDumpBuffer() */
//
// Dump a screen's worth of data directly to the display
// Try to speed it up by comparing the new bytes with the existing buffer
//
int obdDumpBuffer(OBDISP *pOBD, uint8_t *pBuffer, int bRefresh, int bWait, int bFast)
{
int x, y;
int iLines;
    
    if (pOBD->type == LCD_VIRTUAL) { // wrong function for this type of display
        return OBD_ERROR_BAD_PARAMETER;
    }
    if (pBuffer == NULL) { // dump the internal buffer if none is given
        pBuffer = pOBD->ucScreen;
    }
//    if (pBuffer == NULL) {
//        return; // no backbuffer and no provided buffer
//    }
    
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
  if (pOBD->type == LCD_ST7302 || pOBD->type == LCD_ST7305) // special case for ST7302/ST7305
  {
      return ST7302DumpBuffer(pOBD, pBuffer);
  }
#endif // !WIMPY_MCU
  if (pOBD->chip_type == OBD_CHIP_SHARP) // special case for Sharp Memory LCDs
  {
    return SharpDumpBuffer(pOBD, pBuffer);
  }
  iLines = (pOBD->native_height+7) >> 3;
    // 0/180 we can send the 8 lines of pixels straight through
    if (pOBD->iOrientation == 0 || pOBD->iOrientation == 180) {
        if (pOBD->iOrientation == 0 || (pOBD->iOrientation == 180 && pOBD->can_flip)) {
            for (y=0; y<iLines; y++) {
                obdSetPosition(pOBD, 0, y*8, 1);
                RawWriteData(pOBD, pBuffer, pOBD->width);
                pBuffer += pOBD->width;
            }
        } else { // have to manually do 180 flipped
            for (y=iLines-1; y>=0; y--) {
                obdSetPosition(pOBD, 0, y*8, 1);
                for (int x=0; x<pOBD->width; x++) {
                    u8Cache[pOBD->width-1-x] = ucMirror[pBuffer[x]];
                }
                RawWriteData(pOBD, u8Cache, pOBD->width);
                pBuffer += pOBD->width;
            }
        }
     return OBD_SUCCESS;
  } else { // must be 90/270
      // Capture the pixels 'sideways' and send a line at a time
     if (pOBD->iOrientation == 90 || (pOBD->iOrientation == 270 && pOBD->can_flip)) {
          for (x=0; x<pOBD->width; x+=8) {
              uint8_t *s, *d, uc, ucMask, ucTemp[132];
              int j;
              d = ucTemp;
              *d++ = 0x40;
              s = &pBuffer[x + ((pOBD->height-7)>>3)*pOBD->width];
              ucMask = 0x80;
              for (y=0; y<pOBD->height; y++) {
                  uc = 0;
                  for (j=7; j>=0; j--) {
                      uc <<= 1;
                      if (s[j] & ucMask) uc |= 1;
                  } // for j
                  *d++ = uc;
                  ucMask >>= 1;
                  if (ucMask == 0) {
                      ucMask = 0x80;
                      s -= pOBD->width;
                  }
              } // for y
              obdSetPosition(pOBD, 0, x, 1);
              RawWrite(pOBD, ucTemp, pOBD->height + 1);
          } // for x
      } else { // 270 degrees and can't flip 180
          for (x=pOBD->width-8; x>=0; x-=8) {
              uint8_t j, *s, *d, uc, ucMask, ucTemp[132];
              d = ucTemp;
              *d++ = 0x40;
              s = &pBuffer[x];
              ucMask = 1;
              for (y=0; y<pOBD->height; y++) {
                  uc = 0;
                  for (j=0; j<8; j++) {
                      uc <<= 1;
                      if (s[j] & ucMask) uc |= 1;
                  } // for j
                  *d++ = uc;
                  ucMask <<= 1;
                  if (ucMask == 0) {
                      ucMask = 1;
                      s += pOBD->width;
                  }
              } // for y
              obdSetPosition(pOBD, 0, pOBD->width-8-x, 1);
              RawWrite(pOBD, ucTemp, pOBD->height + 1);
          } // for x
      }
  } // 90/270 degrees rotated
    return OBD_SUCCESS;
} /* obdDumpBuffer() */

// Send a single byte command to the OLED controller
void obdWriteCommand(OBDISP *pOBD, unsigned char c)
{
#ifndef MEMORY_ONLY
unsigned char buf[4];

    buf[0] = 0x00; // command introducer
    buf[1] = c;
    RawWrite(pOBD, buf, 2);
#endif // MEMORY_ONLY
} /* obdWriteCommand() */
//
// Send commands to position the "cursor" (aka memory write address)
// to the given row and column
//
void obdSetPosition(OBDISP *pOBD, int x, int y, int bRender)
{
unsigned char buf[4];
int iPitch = pOBD->width;

#if !defined( WIMPY_MCU ) && !defined(__AVR__)
    if (pOBD->type == LCD_ST7302 || pOBD->type == LCD_ST7305) { // special case for ST7302/ST7305
        obdST7302SetPos(pOBD, x, y);
    }
#endif // !WIMPY_MCU
    y >>= 3; // DEBUG - since we address the display by lines of 8 pixels
  pOBD->iScreenOffset = (y*iPitch)+x;

  if (pOBD->type == LCD_VIRTUAL || pOBD->chip_type == OBD_CHIP_SHARP)
    return; // nothing to do
  if (!bRender)
      return; // don't send the commands to the OLED if we're not rendering the graphics now
    obdCachedFlush(pOBD, bRender); // flush any cached data first
  if (pOBD->type == LCD_NOKIA5110)
  {
      obdWriteCommand(pOBD, 0x40 | y);
      obdWriteCommand(pOBD, 0x80 | x);
      return;
  }
  if (pOBD->type == OLED_80x128) // visible portion starts at column 24, row 0
  {
    x += 24;
  }
  else if (pOBD->type == LCD_32x64)
  {
      x += 50;
  }
  else if (pOBD->type == OLED_64x32) // visible display starts at column 32, row 4
  {
    x += 32; // display is centered in VRAM, so this is always true
    if (pOBD->flip == 0) // non-flipped display starts from line 4
       y += 4;
  }
  else  if (pOBD->type == OLED_64x48) {
      x += 32;
      if (!pOBD->flip)
          y += 2; // 128x64 internal RAM
  }
  else if (pOBD->type == LCD_UC1701 && pOBD->flip)
  {
    x += 4;
  }
  else if (pOBD->type == OLED_132x64) // SH1106 has 128 pixels centered in 132
  {
    x += 2;
  }
  else if (pOBD->type == OLED_96x16) // visible display starts at line 2
  { // mapping is a bit strange on the 96x16 OLED
    if (pOBD->flip)
      x += 32;
    else
      y += 2;
  }
  else if (pOBD->type == OLED_72x40) // starts at x=28,y=3
  {
    x += 28;
    if (!pOBD->flip)
    {
      y += 3;
    }
  }
    if (pOBD->com_mode == COM_I2C) { // I2C device
      buf[0] = 0x00; // command introducer
      buf[1] = 0xb0 | y; // set page to Y
      buf[2] = x & 0xf; // lower column address
      buf[3] = 0x10 | (x >> 4); // upper column addr
      RawWrite(pOBD, buf, 4);
    } else { // SPI mode
        obdWriteCommand(pOBD, 0xb0 | y); // set Y
        obdWriteCommand(pOBD, 0x10 | (x >> 4)); // set X MSB
        obdWriteCommand(pOBD, 0x00 | (x & 0xf)); // set X LSB
    }
} /* obdSetPosition() */

//
// Write a block of pixel data to the OLED
// Length can be anything from 1 to 1024 (whole display)
//
void obdWriteDataBlock(OBDISP *pOBD, unsigned char *ucBuf, int iLen, int bRender)
{
int iPitch, iBufferSize;
    
  iPitch = pOBD->width;
  iBufferSize = iPitch * ((pOBD->height+7) / 8);
// Keep a copy in local buffer
if (pOBD->ucScreen && (iLen + pOBD->iScreenOffset) <= iBufferSize)
{
  memcpy(&pOBD->ucScreen[pOBD->iScreenOffset], ucBuf, iLen);
  pOBD->iScreenOffset += iLen;
  // wrap around ?
  if (pOBD->iScreenOffset >= iBufferSize)
    pOBD->iScreenOffset -= iBufferSize;
}
if (pOBD->type == LCD_VIRTUAL || pOBD->chip_type == OBD_CHIP_SHARP)
  return; // nothing else to do
#ifndef MEMORY_ONLY
  if (bRender) {
      RawWriteData(pOBD, ucBuf, iLen);
  }
#endif // MEMORY_ONLY
} /* obdWriteDataBlock() */
