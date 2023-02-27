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
#if defined(_LINUX_) || defined(ARDUINO_ARCH_MCS51)
#define memcpy_P memcpy
#endif
void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS);
void _delay(int iDelay);
void EPDSetPosition(OBDISP *pOBD, int x, int y, int cx, int cy);
static void EPDWaitBusy(OBDISP *pOBD, int bQuick);
static void EPDSleep(OBDISP *pOBD);
static void EPDFill(OBDISP *pOBD, uint8_t ucCMD, uint8_t ucPattern);
#ifndef WIMPY_MCU
static void EPDWriteImage4bpp(OBDISP *pOBD, uint8_t ucCMD, int x, int y, int w, int h);
static void EPDWriteImage(OBDISP *pOBD, uint8_t ucCMD, uint8_t *pBits, int x, int y, int w, int h, int bRed);
#endif
void EPD213_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial);
void EPD293_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial);
void EPD154_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial);
void EPD_CMD2(OBDISP *pOBD, uint8_t cmd, uint8_t param);
void obdSetDCMode(OBDISP *pOBD, int iMode);
void InvertBytes(uint8_t *pData, uint8_t bLen);
void SPI_BitBang(OBDISP *pOBD, uint8_t *pData, int iLen, uint8_t iMOSIPin, uint8_t iSCKPin);
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen);
static void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen);
// EPD look up tables
// new waveform created by Jean-Marc Zingg for the actual panel
#define T1 25 // color change charge balance pre-phase
#define T2  1 // color change or sustain charge balance pre-phase
#define T3  2 // color change or sustain phase
#define T4 25 // color change phase

const unsigned char lut_20_vcom0_partial[] PROGMEM =
{
  0x00, T1, T2, T3, T4, 1, // 00 00 00 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char lut_21_ww_partial[] PROGMEM =
{ // 10 w
  0x18, T1, T2, T3, T4, 1, // 00 01 10 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char lut_22_bw_partial[] PROGMEM =
{ // 10 w
  0x5A, T1, T2, T3, T4, 1, // 01 01 10 10
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char lut_23_wb_partial[] PROGMEM =
{ // 01 b
  0xA5, T1, T2, T3, T4, 1, // 10 10 01 01
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char lut_24_bb_partial[] PROGMEM =
{ // 01 b
  0x24, T1, T2, T3, T4, 1, // 00 10 01 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

// 2.7" 176x264 LUTs
const uint8_t lut_fast_vcom_dc[] PROGMEM = {
    0x00, 0x00,
    0x00, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x00, 0x0E, 0x01, 0x0E, 0x01, 0x01,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};
const uint8_t lut_fast_ww[] PROGMEM = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x40, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x01,
    0x80, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};
const uint8_t lut_fast_bw[] PROGMEM = {
    0xA0, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x01,
    0x90, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0xB0, 0x04, 0x10, 0x00, 0x00, 0x05,
    0xB0, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0xC0, 0x23, 0x00, 0x00, 0x00, 0x01
};
const uint8_t lut_fast_bb[] PROGMEM = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x40, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x01,
    0x80, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};
const uint8_t lut_fast_wb[] PROGMEM = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x20, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x01,
    0x10, 0x0A, 0x0A, 0x00, 0x00, 0x01,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

const uint8_t lut_vcom_dc[] PROGMEM =
{
    0x00, 0x00,
    0x00, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x32, 0x32, 0x00, 0x00, 0x02,
    0x00, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const uint8_t lut_ww[] PROGMEM =
{
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
     0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t lut_bw[] PROGMEM =
{
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
     0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const uint8_t lut_bb[] PROGMEM =
{
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
      0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
      0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t lut_wb[] PROGMEM =
{
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
     0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_vcom0_full[] PROGMEM =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_ww_full[] PROGMEM =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_bw_full[] PROGMEM =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char lut_wb_full[] PROGMEM =
{
  0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_bb_full[] PROGMEM =
{
  0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char epd102_lut_w_full[] PROGMEM =
{
  0x60, 0x5A, 0x5A, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char epd102_lut_b_full[] PROGMEM =
{
  0x90, 0x5A, 0x5A, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#ifndef WIMPY_MCU
const uint8_t epd583r_init_sequence_full[] PROGMEM = {
    0x03, 0x01, 0x37, 0x00, // power setting
    0x03, 0x00, 0xcf, 0x08, // panel setting
    0x04, 0x06, 0xc7, 0xcc, 0x28, // boost
    0x02, 0x30, 0x3a, // PLL 15s refresh
    0x02, 0x41, 0x00, // temperature
    0x02, 0x50, 0x77, // VCOM
    0x02, 0x60, 0x22, // TCON
    0x05, 0x61, 0x02, 0x58, 0x01, 0xc0, // RES
    0x02, 0x82, 0x28, // temp range
    0x02, 0xe5, 0x03, // flash mode
    0x01, 0x04, // power on
    BUSY_WAIT,
    0x00
};
const uint8_t epd74r_init_sequence_full[] PROGMEM = {
    0x02, 0x65, 0x01,
    0x01, 0xab,
    0x02, 0x65, 0x00,
    0x01, 0x04,
    BUSY_WAIT,
    0x03, 0x01, 0x37, 0x00, // 0x05, 0x05,
    0x03, 0x00, 0xcf, 0x08,
//    0x02, 0xe5, 0x03,
//    0x02, 0x03, 0x00,
    0x04, 0x06, 0xc7, 0xcc, 0x28,
    0x02, 0x30, 0x3c,
    0x02, 0x41, 0x00,
    0x02, 0x50, 0x77,
    0x02, 0x60, 0x22,
    0x05, 0x61, 0x02, 0x80, 0x01, 0x80,
//    0x02, 0x65, 0x01,
//    0x02, 0x65, 0x00,
//    0x01, 0x40,
//    BUSY_WAIT,
//    0x02, 0x8d, 0x80,
    0x02, 0x82, 0x1e,
    0x02, 0xe5, 0x03,
//    0x01, 0x02,
//    BUSY_WAIT,
    0x01, 0x04,
    BUSY_WAIT,
//    0x02, 0x65, 0x01,
//    0x02, 0x65, 0x00,
//    0x01, 0x40,
//    BUSY_WAIT,
//    0x02, 0x8d, 0xc0,
//    0x02, 0x30, 0x2a,
    0x00
};
#endif // !WIMPY_MCU

const uint8_t epd35r_init_sequence_full[] PROGMEM = {
    0x01, 0x12, // SW RESET
    BUSY_WAIT,
    0x02, 0x3c, 0x05,
    0x02, 0x18, 0x80,
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x03, 0x1b, 0x17, 0x80, // reading temp, delay 5ms
    BUSY_WAIT,
    0x02, 0x4e, 0x00, // ram counter start
    0x03, 0x4f, 0x7f, 0x01,
    0x00
};

// init sequence for GDEW042Z15
const uint8_t epd42r2_init_sequence_full[] PROGMEM = {
    0x03, UC8151_PSR, 0xcf, 0x8d,
   0x06, UC8151_PWR, 0x03, 0x10, 0x3f, 0x3f, 0x0d,
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x05, UC8151_TRES, 0x01, 0x90, 1, 0x2c, // resolution
    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
//    0x02, 0x00, 0x0f, // LUT from OTP
    0x02, UC8151_CDI, 0x5c, // inverted, white border
    0x00
};

// init sequence for 2.13" Random screen double connector
const uint8_t EPD213R_104x212_d_init_sequence_full[] PROGMEM = {
    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x02, 0x00, 0x0f, // LUT from OTP
    0x02, UC8151_CDI, 0x5c, // inverted, white border
    0x00
};

// initialization sequence for 3.7" 240x416 e-paper
const uint8_t epd37_init_sequence_full[] PROGMEM = {
    0x03, UC8151_PSR, 0xdf, 0x8d,
    0x06, UC8151_PWR, 0x03, 0x10, 0x3f, 0x3f, 0x0d,
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x04, UC8151_TRES, 0xf0, 1, 0xa0, // resolution

    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
    0x02, UC8151_PFS, 0x00, // FRAMES_1
    0x02, UC8151_TSE, 0x00, // TEMP_INTERNAL | OFFSET_0
    0x02, UC8151_TCON, 0x22,
    0x02, UC8151_CDI, 0xd7, // inverted, white border
    0x02, UC8151_PLL, 0x09, // HZ_50
    0x00 // end of table
};
// initialization sequence for 2.9" 296x128 e-paper
const uint8_t epd29_init_sequence_full[] PROGMEM = {
    0x02, UC8151_PSR, 0x80 | 0x00 | 0x10 | 0x08 | 0x04 | 0x02 | 0x01, // RES_128x296, LUT_OTP, FORMAT_BW, SHIFT_LEFT, BOOSTER_ON, RESET_NONE
    0x06, UC8151_PWR, 0x03, 0x00, 0x2b, 0x2b, 0x2b,
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
    0x02, UC8151_PFS, 0x00, // FRAMES_1
    0x04, UC8151_TRES, 0x80, 1, 0x28, // resolution
    0x02, UC8151_TSE, 0x00, // TEMP_INTERNAL | OFFSET_0
    0x02, UC8151_TCON, 0x22,
    0x02, UC8151_CDI, 0x9c, // inverted, white border
    0x02, UC8151_PLL, 0x3a, // HZ_100
    0x00 // end of table
};

// initialization sequence for 1.02" 80x128 e-paper
const uint8_t epd102_init_sequence_full[] PROGMEM = {
//    0x02, 0xd2, 0x3f, // ?
    0x02, 0x00, 0x5f,
//    0x05, 0x01, 0x03, 0x00, 0x2b, 0x2b,
//    0x02, 0x06, 0x3f,
    0x03, 0x2a, 0x00, 0x00,
//    0x02, 0x30, 0x13, // PLL 30Hz
//    0x02, 0x50, 0x57, // VCOM default
//    0x02, 0x60, 0x22, // TCON 24us
//    0x03, 0x61, 0x50,0x80, // resolution
//    0x02, 0x82, 0x12, // VCOM -1v
//    0x02, 0xe3, 0x33, // power saving
    0x01, 0x04, // power on
    BUSY_WAIT,
    0x02, 0x50, 0x97, // panel setting
    0x00 // end of table
};

const uint8_t epd42r_init_sequence[] PROGMEM = {
    0x01, 0x12, // soft reset
    BUSY_WAIT,
    0x02, 0x74, 0x54, // set analog block control
    0x02, 0x7e, 0x3b, // set digital block control
    0x03, 0x2b, 0x04, 0x63, // ACVCOM
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f, // Softstart
    0x04, 0x01, 0x2b, 0x01, 0x00, // output control
    0x02, 0x11, 0x03, // data entry mode
    0x03, 0x44, 0x00, 0x31, // RAM X start/end
    0x05, 0x45, 0,0,0x2b, 0x01, // RAM Y start/end
    0x02, 0x3c, 0x01, // border (0=bk,1=wh,2=red)
    0x02, 0x18, 0x80, // temp sensor = internal
    0x02, 0x21, 0x00, // display update ctrl 1
    0x02, 0x22, 0xb1, // display update ctrl 2
    0x01, 0x20, // master activation
    BUSY_WAIT,
    0x02, 0x4e, 0x00, // RAM X counter
    0x03, 0x4f, 0x2b, 0x01, // RAM Y counter
    0x00
};
const uint8_t epd29r_init_sequence[] PROGMEM = {
    0x01, 0x12, // soft reset
    BUSY_WAIT,
    0x02, 0x74, 0x54, // set analog block control
    0x02, 0x7e, 0x3b, // set digital block control
    0x03, 0x2b, 0x04, 0x63, // ACVCOM
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f, // Softstart
    0x04, 0x01, 0x27, 0x01, 0x00, // output control
    0x02, 0x11, 0x03, // data entry mode
    0x03, 0x44, 0x00, 0x0f, // RAM X start/end
    0x05, 0x45, 0,0,0x27, 0x01, // RAM Y start/end
    0x02, 0x3c, 0x01, // border (0=bk,1=wh,2=red)
    0x02, 0x18, 0x80, // temp sensor = internal
    0x02, 0x21, 0x00, // display update ctrl 1
    0x02, 0x22, 0xb1, // display update ctrl 2
    0x01, 0x20, // master activation
    BUSY_WAIT,
    0x02, 0x4e, 0x00, // RAM X counter
    0x03, 0x4f, 0x27, 0x01, // RAM Y counter
    0x00
};

// for 152x152 BWR
const uint8_t epd29r_init_sequence_152[] PROGMEM = {
    0x01, 0x12, // soft reset
    BUSY_WAIT,
    0x02, 0x74, 0x54, // set analog block control
    0x02, 0x7e, 0x3b, // set digital block control
    0x03, 0x2b, 0x04, 0x63, // ACVCOM
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f, // Softstart
    0x04, 0x01, 0x97, 0x00, 0x00, // output control
    0x02, 0x11, 0x03, // data entry mode
    0x03, 0x44, 0x00, 0x12, // RAM X start/end
    0x05, 0x45, 0,0,0x97, 0x00, // RAM Y start/end
    0x02, 0x3c, 0x01, // border (0=bk,1=wh,2=red)
    0x02, 0x18, 0x80, // temp sensor = internal
    0x02, 0x21, 0x00, // display update ctrl 1
    0x02, 0x22, 0xb1, // display update ctrl 2
    0x01, 0x20, // master activation
    BUSY_WAIT,
    0x02, 0x4e, 0x00, // RAM X counter
    0x03, 0x4f, 0x97, 0x00, // RAM Y counter
    0x00
};
const uint8_t epd29_init_sequence_partial[] PROGMEM = {
    0x02, UC8151_PSR, 0x80 | 0x20 | 0x10 | 0x00 | 0x02 | 0x01, // RES_128x296, LUT_REG, FORMAT_BW, SHIFT_LEFT, BOOSTER_ON, RESET_NONE
    0x06, UC8151_PWR, 0x03, 0x00, 0x2b, 0x2b, 0x03,
    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x02, UC8151_PLL, 0x3a, // HZ_100
    0x02, UC8151_VDCS, 0x12, // VCOM DC setting
//    0x02, UC8151_CDI, 0x97,
    0x2d, UC8151_LUT_VCOM, // VCOM LUT
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        ,0x00, 0x00,
    0x2b, UC8151_LUT_WW, // WW LUT
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_BW,
        0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_WB,
        0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_BB,
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00 // end of table
};
// 1.54" 152x152
const uint8_t epd154b_init_sequence_full[] PROGMEM =
{
    1, UC8151_PON,
    BUSY_WAIT,
    2, UC8151_PSR, 0xdf, // panel setting
    4, UC8151_TRES, 0x98, 0, 0x98, // resolution
    2, UC8151_CDI, 0x97, // VCOM
    0
};
const uint8_t epd154b_init_sequence_fast[] PROGMEM =
{
    1, UC8151_PON,
    BUSY_WAIT,
    2, UC8151_PSR, 0x3f, // panel setting
    6, UC8151_PWR, 0x03, 0x00, 0x21, 0x21, 0x03,
    4, UC8151_TRES, 0x98, 0, 0x98, // resolution
    2, UC8151_VDCS, 0x12,
    2, UC8151_CDI, 0x17, // VCOM
    37, 0x20, 0x00, 10, 0, 60, 10, 1, // VCOM LUT
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
    37, 0x21, 0x00, 10, 0, 60, 10, 1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
    37, 0x22, 0x5a, 10, 0, 60, 10, 1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
    37, 0x23, 0x84, 10, 0, 60, 10, 1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
    37, 0x24, 0x00, 10, 0, 60, 10, 1,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
    0
};

// 1.54" 200x200
const uint8_t epd154_init_sequence_full[] PROGMEM =
{
    1, SSD1608_SW_RESET,
    BUSY_WAIT,
    4, SSD1608_DRIVER_CONTROL, 199, 0, 0,
    2, SSD1608_DATA_MODE, 0x03,
    2, SSD1608_WRITE_BORDER, 0x01,
    0
};
const uint8_t epd213b_init_sequence_full[] PROGMEM =
{
    0x02, 0x74, 0x54,
    0x02, 0x7e, 0x3b,
    0x03, 0x2b, 0x04, 0x63,
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f,
    0x04, 0x01, 0xf9, 0x00, 0x00,
    0x02, 0x11, 0x03,
    0x03, 0x44, 0x00, 0x0f,
    0x05, 0x45, 0x00, 0x00, 0xf9, 0x00,
    0x02, 0x3c, 0xc0,
    0x02, 0x18, 0x80,
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x02, 0x4e, 0x00,
    0x03, 0x4f, 0x00, 0x00,
    0x00 // end of table
}; /* epd213b_init_sequence_full[] */

const uint8_t epd293_init_sequence_full[] PROGMEM =
{
//    0x01, SSD1608_SW_RESET,
//    BUSY_WAIT,
    0x02, 0x74, 0x54,
    0x02, 0x7e, 0x3b,
    0x03, 0x2b, 0x04, 0x63,
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f,
    0x04, 0x01, 0x27, 0x01, 0x00,
    0x02, 0x11, 0x03,
    0x03, 0x44, 0x00, 0x0f,
    0x05, 0x45, 0x00, 0x00, 0x27, 0x01,
    0x02, 0x3c, 0xc0,
    0x02, 0x18, 0x80,
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x02, 0x4e, 0x00,
    0x03, 0x4f, 0x00, 0x00,
    0x00 // end of table
}; /* epd293_init_sequence_full[] */

const uint8_t epd266_init_sequence_full[] PROGMEM =
{
    0x04, 0x01, 0x27, 0x01, 0x00, // driver output control
    0x02, 0x11, 0x03, // data entry mode
    0x03, 0x44, 0x00, 0x0f, // ram address
    0x05, 0x45, 0x00, 0x00, 0x27, 0x01,
    0x02, 0x3c, 0x05, // border color
    0x03, 0x21, 0x00, 0x80, // display update ctrl
    0x02, 0x18, 0x80, // read built-in temp sensor
    0x02, 0x4e, 0x00, // ram counter x
    0x03, 0x4f, 0x00, 0x00, // ram counter y

    0x02, 0x74, 0x54,
    0x02, 0x7e, 0x3b,
    0x03, 0x2b, 0x04, 0x63,
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f,
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x00 // end of table
}; /* epd266_init_sequence_full[] */

const uint8_t epd27_init_sequence_full[] PROGMEM =
{
    0x02, 0x11, 0x03, // data entry mode
    0x03, 0x44, 0x00, 0x15, // ram address
    0x05, 0x45, 0x00, 0x00, 0x07, 0x01,
    0x02, 0x3c, 0x80, // border color
    0x02, 0x18, 0x80, // read built-in temp sensor
    0x02, 0x4e, 0x00, // ram counter x
    0x03, 0x4f, 0x00, 0x00, // ram counter y
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x00 // end of table
}; /* epd27_init_sequence_full[] */

const uint8_t epd579_init_sequence_full[] PROGMEM =
{
    2, 0x11, 0x03, // data format
    2, 0x3c, 0x80, // border color
    0x00 // end of table
}; /* epd579_init_sequence_full[] */

const uint8_t epd29b_init_sequence_full[] PROGMEM = {
    1, SSD1608_SW_RESET,
    BUSY_WAIT,
    2, 0x74, 0x54,
    2, 0x7e, 0x3b,
    4, 0x01, 39, 1, 0, // gate setting (height -1)
    2, 0x11, 0x3, // data entry mode
    2, 0x3c, 0x03, // black border
    2, 0x2c, 0x55, // VCOM
    2, 0x03, 0x17, // gate drive voltage
    4, 0x04, 0x41, 0xac, 0x32, // source driving voltage
    2, 0x3a, 0x07, // dummy line period
    2, 0x3b, 0x04, // get line width
    2, 0x4e, 0x00, // ram x start
    3, 0x4f, 0x00, 0x00, // ram y start
    BUSY_WAIT,
    0
};
const uint8_t epd213_104x212_init_sequence_full[] PROGMEM = {
    1, SSD1608_SW_RESET,
    BUSY_WAIT,
    2, 0x74, 0x54,
    2, 0x7e, 0x3b,
    4, 0x01, 211, 0, 0, // gate setting (height -1)
    2, 0x11, 0x3, // data entry mode
    2, 0x3c, 0x03, // black border
    2, 0x2c, 0x55, // VCOM
    2, 0x03, 0x17, // gate drive voltage
    4, 0x04, 0x41, 0xac, 0x32, // source driving voltage
    2, 0x3a, 0x07, // dummy line period
    2, 0x3b, 0x04, // get line width
    2, 0x4e, 0x00, // ram x start
    3, 0x4f, 0x00, 0x00, // ram y start
    BUSY_WAIT,
    0
};

const uint8_t epd213_122x250_init_sequence_full[] PROGMEM = {
    1, SSD1608_SW_RESET,
    BUSY_WAIT,
    2, 0x74, 0x54,
    2, 0x7e, 0x3b,
    4, 0x01, 249, 0, 0, // gate setting (height -1)
    2, 0x11, 0x3, // data entry mode
    2, 0x3c, 0x03, // black border
    2, 0x2c, 0x55, // VCOM
    2, 0x03, 0x15, // gate drive voltage
    4, 0x04, 0x41, 0xa8, 0x32, // source driving voltage
    2, 0x3a, 0x30, // dummy line period
    2, 0x3b, 0x0a, // get line width
    2, 0x4e, 0x00, // ram x start
    3, 0x4f, 0x00, 0x00, // ram y start
    BUSY_WAIT,
    0
};
const uint8_t epd42_init_sequence_full[] PROGMEM = {
    2, UC8151_PSR, 0x9f,
    6, UC8151_PWR, 0x03, 0x00, 0x2b, 0x2b, 0x2b,
    1, UC8151_PON,
    BUSY_WAIT,
    4, UC8151_BTST, 0x17, 0x17, 0x17,
    2, UC8151_PFS, 0x00,
    2, UC8151_TSE, 0x00,
    2, UC8151_TCON, 0x22,
    2, UC8151_CDI, 0xd7,
    2, UC8151_PLL, 0x3a,
    5, UC8151_TRES, 0x01, 0x90, 0x01, 0x2c,
    2, UC8151_VDCS, 0x12,
    0
};
const uint8_t epd42_init_sequence_partial[] PROGMEM = {
    0x02, UC8151_PSR, 0x80 | 0x20 | 0x10 | 0x00 | 0x02 | 0x01, // RES_128x296, LUT_REG, FORMAT_BW, SHIFT_LEFT, BOOSTER_ON, RESET_NONE
    0x06, UC8151_PWR, 0x03, 0x00, 0x2b, 0x2b, 0x03,
    0x04, UC8151_BTST, 0x17,0x17,0x17, // booster soft-start config - START_10MS | STRENGTH_3 | OFF_6_58US
    0x01, UC8151_PON, // power on
    BUSY_WAIT,
    0x02, UC8151_PLL, 0x3a, // HZ_100
    0x02, UC8151_VDCS, 0x12, // VCOM DC setting
//    0x02, UC8151_CDI, 0x97,
    0x2d, UC8151_LUT_VCOM, // VCOM LUT
    0x00, 0x0E, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_WW, // WW LUT
    0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_BW,
    0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_WB,
    0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, UC8151_LUT_BB,
    0x50, 0x0E, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00 // end of table
};
#ifndef WIMPY_MCU
const uint8_t st7302_wenting[] PROGMEM PROGMEM = {
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
// Write a single line to a Sharp memory LCD
// You must provide the exact number of bytes needed for a complete line
// e.g. for the 144x168 display, pSrc must provide 144 pixels (18 bytes)
//
void obdWriteLCDLine(OBDISP *pOBD, uint8_t *pSrc, int iLine)
{
    int x;
    uint8_t c, ucInvert, *d, ucStart;
    uint8_t ucLineBuf[54]; // 400 pixels is max supported width = 50 bytes + 4
    int iPitch = pOBD->width / 8;
    static int iVCOM = 0;

//    if (pOBD == NULL || pSrc == NULL || pOBD->type < SHARP_144x168)
//        return; // invalid request
    if (iLine < 0 || iLine >= pOBD->height)
        return;
    
      ucInvert = (pOBD->invert) ? 0x00 : 0xff;
      digitalWrite(pOBD->iCSPin, HIGH); // active high

      ucStart = 0x80; // write command
      iVCOM++;
      if (iVCOM & 0x100) // flip it every 256 lines
        ucStart |= 0x40; // VCOM bit
      ucLineBuf[1] = ucStart;
      // this code assumes I2C, so the first byte is ignored
      RawWrite(pOBD, ucLineBuf, 2); // write command(01) + vcom(02)

     d = &ucLineBuf[2];
     ucLineBuf[1] = pgm_read_byte(&ucMirror[iLine+1]); // current line number
     for (x=0; x<iPitch; x++)
     {
         c = pSrc[0] ^ ucInvert; // we need to brute-force invert it
         *d++ = pgm_read_byte(&ucMirror[c]);
         pSrc++;
     } // for x
    // write this line to the display
    ucLineBuf[iPitch+2] = 0; // end of line
    RawWrite(pOBD, ucLineBuf, iPitch+3);
    ucLineBuf[1] = 0;
    RawWrite(pOBD, ucLineBuf, 2); // final transfer
    digitalWrite(pOBD->iCSPin, LOW); // de-activate
} /* obdWriteLCDLine() */

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
    int iLen;
    uint8_t *s;
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
    else // Nokia 5110
    {
        s = (uint8_t *)nokia5110_initbuf;
        iLen = sizeof(nokia5110_initbuf);
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
// More efficient means of sending commands, data and busy-pauses
//
void EPDSendCMDSequence(OBDISP *pOBD, const uint8_t *pSeq)
{
int iLen;
uint8_t *s;
    
    s = (uint8_t *)pSeq;
    while (pgm_read_byte(s) != 0) {
        iLen = pgm_read_byte(s++);
        if (iLen == BUSY_WAIT) {
            EPDWaitBusy(pOBD, 1);
        } else {
            obdWriteCommand(pOBD, pgm_read_byte(s));
            s++;
            memcpy_P(u8Cache, s, iLen-1);
            s += (iLen-1);
            RawWriteData(pOBD, u8Cache, iLen-1);
        }
    } // while more commands to send
} /* EPDSendCMDSequence() */

void EPD29_Init(OBDISP *pOBD)
{
uint8_t ucTemp[4];
    
    obdWriteCommand(pOBD, UC8151_PON); // power on
    EPDWaitBusy(pOBD, 1);
    EPD_CMD2(pOBD, UC8151_PSR, 0x80 | 0x10 | 0x00 | 0x02 | 0x01); // panel setting
    obdWriteCommand(pOBD, UC8151_TRES); // resolution
    ucTemp[0] = (uint8_t)pOBD->native_width;
    ucTemp[1] = (uint8_t)(pOBD->native_height >> 8);
    ucTemp[2] = (uint8_t)(pOBD->native_height & 0xff);
    RawWriteData(pOBD, ucTemp, 3);
//    obdWriteCommand(pOBD, UC8151_CDI);
    EPDSleep(pOBD);
} /* EPD29_Init() */

const unsigned char lut_w_full[] PROGMEM =
{
  0x60, 0x5A, 0x5A, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char lut_b_full[] PROGMEM =
{
  0x90, 0x5A, 0x5A, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#ifdef FUTURE
const uint8_t epd213_lut[] PROGMEM =
{
	// black
	0x48, 0x50, 0x10, 0x10, 0x13, 0x00, 0x00,
	0x48, 0x50, 0x80, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x48, 0x55, 0x00, 0xbb, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x10, 0x04, 0x04, 0x04, 0x04,
	0x10, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x08, 0x08, 0x10, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

static const uint8_t epd293_lut_partial[] PROGMEM = {
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x0A, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
#ifdef FUTURE
//lut0 (KEEP) voltages
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//lut1 (W2B) voltages
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//lut2 (B2W) voltages
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//lut3 (unused) voltages
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//lut4 (vcom) voltages
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

//group0 phase lengths and repeat count
0xa, 0x00, 0x00, 0x00, 0x00,

//group1 not used
0x00, 0x00, 0x00, 0x00, 0x00,

//group2 not used
0x00, 0x00, 0x00, 0x00, 0x00,

//group3 phase lengths and repeat count
0x00, 0x00, 0x00, 0x00, 0x00,

//group4 phase lengths and repeat count
0x00, 0x00, 0x00, 0x00, 0x00,

//group5 phase lengths and repeat count
0x00, 0x00, 0x00, 0x00, 0x00,

//group6 phase lengths and repeat count
0x00, 0x00, 0x00, 0x00, 0x00
#endif
};

const uint8_t epd213_lut2_partial[] PROGMEM =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
    0x40,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7

    0x0A,0x00,0x00,0x00,0x00,                       // TP0 A~D RP0
    0x00,0x00,0x00,0x00,0x00,                       // TP1 A~D RP1
    0x00,0x00,0x00,0x00,0x00,                       // TP2 A~D RP2
    0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
    0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
    0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
    0x00,0x00,0x00,0x00,0x00,                       // TP6 A~D RP6

};

const uint8_t epd213_lut2_full[] PROGMEM =
{
	// black
	0x80,0x60,0x40,0x00,0x00,0x00,0x00,
	0x10,0x60,0x20,0x00,0x00,0x00,0x00,
	0x80,0x60,0x40,0x00,0x00,0x00,0x00,
	0x10,0x60,0x20,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x03,0x03,0x00,0x00,0x02,
	0x09,0x09,0x00,0x00,0x02,
	0x03,0x03,0x00,0x00,0x02,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00
};

const uint8_t lut154_full_update[] PROGMEM =
    {
        0x80,0x48,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x40,0x48,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x80,   0x48,   0x40,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x40,   0x48,   0x80,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0xA,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x8,    0x1,    0x0,    0x8,    0x1,    0x0,    0x2,
        0xA,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
        0x22,   0x22,   0x22,   0x22,   0x22,   0x22,   0x0,    0x0,    0x0
    };

const uint8_t lut154_partial_update[] PROGMEM =
{
    0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0xF,0x0,0x0,0x0,0x0,0x0,0x0,
    0x1,0x1,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0
};

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
        digitalWrite(pOBD->iCSPin, HIGH); //(pOBD->type < SHARP_144x168)); // set to not-active
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
    if (iLED != -1 && iLED != 0xff)
    {
        if (iType >= EPD42_400x300) // for EPD displays, LED = BUSY
            pinMode(iLED, INPUT_PULLUP);
        else
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
        pOBD->can_flip = 0;
    }
#endif
    if (iType == OLED_80x128)
    {
        pOBD->native_width = pOBD->width = 80;
        pOBD->native_height = pOBD->height = 128;
    }
    else if (iType == SHARP_144x168)
    {
        pOBD->native_width = pOBD->width = 144;
        pOBD->native_height = pOBD->height = 168;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
    }
    else if (iType == SHARP_400x240)
    {
        pOBD->native_width = pOBD->width = 400;
        pOBD->native_height = pOBD->height = 240;
        pOBD->can_flip = 0;
        pOBD->iDCPin = 0xff; // no D/C wire on this display
    }
    else if (iType == EPD27b_176x264)
    {
        pOBD->native_width = pOBD->width = 176;
        pOBD->native_height = pOBD->height = 264;
        pOBD->busy_idle = LOW;
        pOBD->can_flip = 0;
        pOBD->chip_type = OBD_CHIP_SSD16xx;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->pInitFull = epd27_init_sequence_full;
        pOBD->type = EPD293_128x296; // same for the rest
        return; // nothing else to do yet
    }
    else if (iType == EPD27_176x264)
    {
        pOBD->native_width = pOBD->width = 176;
        pOBD->native_height = pOBD->height = 264;
        pOBD->busy_idle = HIGH;
        pOBD->can_flip = 0;
        pOBD->chip_type = OBD_CHIP_UC8151;
        pOBD->iFlags |= OBD_CS_EVERY_BYTE; // unfortunately needs this
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        return; // nothing else to do yet
    }
    else if (iType == EPD579_792x272) {
        pOBD->native_width = pOBD->width = 792;
        pOBD->native_height = pOBD->height = 272;
        pOBD->pInitFull = epd579_init_sequence_full;
        pOBD->busy_idle = LOW;
        pOBD->chip_type = OBD_CHIP_SSD16xx;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->iFlags |= OBD_CS_EVERY_BYTE;
        pOBD->can_flip = 0;
    }
    else if (iType == EPD266_152x296) {
        pOBD->native_width = pOBD->width = 152;
        pOBD->native_height = pOBD->height = 296;
        pOBD->pInitFull = epd266_init_sequence_full;
        pOBD->busy_idle = LOW;
        pOBD->chip_type = OBD_CHIP_SSD16xx;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->can_flip = 0;
        pOBD->type = EPD293_128x296; // same for the rest
        return; // nothing else to do yet
    }
    else if (iType == EPD42_400x300)
    { // WFT0420CZ15
        pOBD->native_width = pOBD->width = 400;
        pOBD->native_height = pOBD->height = 300;
        pOBD->busy_idle = HIGH;
        pOBD->chip_type = OBD_CHIP_UC8151;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->can_flip = 0;
        pOBD->pInitFull = epd42_init_sequence_full;
        return; // nothing else to do yet
    }
    else if (iType == EPD293_128x296 || iType == EPD213B_122x250)
    {
        if (iType == EPD213B_122x250) {
            pOBD->native_width = pOBD->width = 122;
            pOBD->native_height = pOBD->height = 250;
            pOBD->type = EPD293_128x296; // the rest is the same
            pOBD->pInitFull = epd213b_init_sequence_full;
        } else {
            pOBD->native_width = pOBD->width = 128;
            pOBD->native_height = pOBD->height = 296;
            pOBD->pInitFull = epd293_init_sequence_full;
        }
        pOBD->busy_idle = LOW;
        pOBD->chip_type = OBD_CHIP_SSD16xx;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->can_flip = 0;
        return; // nothing else to do yet
    }
    else if (iType == EPD213_122x250 || iType == EPD213_104x212 || iType == EPD29B_128x296)
    {
        if (iType == EPD213_122x250) {
            pOBD->native_width = pOBD->width = 122;
            pOBD->native_height = pOBD->height = 250;
            pOBD->pInitFull = epd213_122x250_init_sequence_full;
        } else if (iType == EPD213_104x212) {
            pOBD->native_width = pOBD->width = 104;
            pOBD->native_height = pOBD->height = 212;
            pOBD->pInitFull = epd213_104x212_init_sequence_full;
        } else {
            pOBD->native_width = pOBD->width = 128;
            pOBD->native_height = pOBD->height = 296;
            pOBD->pInitFull = epd29b_init_sequence_full;
        }
        pOBD->can_flip = 0;
        pOBD->busy_idle = LOW;
        pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
        pOBD->chip_type = OBD_CHIP_SSD16xx;
        return;
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
} else if (iType == EPD74R_640x384 || iType == EPD583R_600x448) {
      if (iType == EPD583R_600x448) {
          pOBD->native_width = pOBD->width = 600;
          pOBD->native_height = pOBD->height = 448;
          pOBD->type = EPD74R_640x384;
      } else {
          pOBD->native_width = pOBD->width = 640;
          pOBD->native_height = pOBD->height = 384;
      }
      pOBD->can_flip = 0;
      pOBD->busy_idle = HIGH;
      pOBD->iFlags |= OBD_3COLOR;
//      pOBD->iFlags |= OBD_CS_EVERY_BYTE;
      pOBD->iTimeout = 25000; // 3-color need a longer timeout (25 seconds)
      return;
#endif // !WIMPY_MCU
  } else if (iType == EPD42R2_400x300 || iType == EPD213R_104x212 || iType == EPD37Y_240x416) {
      if (iType == EPD37Y_240x416) {
          pOBD->native_width = pOBD->width = 240;
          pOBD->native_height = pOBD->height = 416;
          pOBD->type = EPD42R2_400x300; // use the same commands
      }else if (iType == EPD213R_104x212) {
          pOBD->native_width = pOBD->width = 104;
          pOBD->native_height = pOBD->height = 212;
          pOBD->type = EPD42R2_400x300; // use the same commands
      } else {
          pOBD->native_width = pOBD->width = 400;
          pOBD->native_height = pOBD->height = 300;
      }
      pOBD->pInitFull = epd42r2_init_sequence_full;
      pOBD->can_flip = 0;
      pOBD->busy_idle = HIGH;
      pOBD->iFlags |= OBD_3COLOR;
      pOBD->chip_type = OBD_CHIP_UC8151;
      pOBD->iTimeout = 25000; // 3-color need a longer timeout (25 seconds)
      EPDSendCMDSequence(pOBD, pOBD->pInitFull);
  } else if (iType == EPD213R_104x212_d) {
      pOBD->native_width = pOBD->width = 104;
      pOBD->native_height = pOBD->height = 212;
      pOBD->type = EPD213R_104x212_d;
      pOBD->can_flip = 0;
      pOBD->busy_idle = HIGH;
      pOBD->iFlags |= OBD_3COLOR;
      pOBD->chip_type = OBD_CHIP_UC8151;
      pOBD->iTimeout = 25000; // 3-color need a longer timeout (25 seconds)
  }
  else if (iType == EPD35R_184x384) {
      pOBD->native_width = pOBD->width = 184;
      pOBD->native_height = pOBD->height = 384;
      pOBD->iTimeout = 25000;
      pOBD->iFlags |= OBD_3COLOR;
      pOBD->chip_type = OBD_CHIP_SSD16xx;
      pOBD->can_flip = 0;
      pOBD->busy_idle = LOW;
      pOBD->pInitFull = epd35r_init_sequence_full;
      return;
  }
  else if (iType == EPD29R_128x296 || iType == EPD154R_152x152 || iType == EPD42R_400x300 || iType == EPD31R_168x296 || iType == EPD154Y_152x152 || iType == EPD29Y_128x296 || iType == EPD213R2_122x250)
  { // BLACK/WHITE/RED
      if (iType == EPD31R_168x296) {
          pOBD->native_width = pOBD->width = 168;
          pOBD->native_height = pOBD->height = 296;
          pOBD->pInitFull = epd29r_init_sequence;
      } else if (iType == EPD29Y_128x296) {
          pOBD->native_width = pOBD->width = 128;
          pOBD->native_height = pOBD->height = 296;
          pOBD->pInitFull = epd29r_init_sequence;
          pOBD->x_offset = 1;
      } else if (iType == EPD29R_128x296) {
          pOBD->native_width = pOBD->width = 128;
          pOBD->native_height = pOBD->height = 296;
          pOBD->pInitFull = epd29r_init_sequence;
      } else if (iType == EPD154Y_152x152) {
          pOBD->native_width = pOBD->width = 152;
          pOBD->native_height = pOBD->height = 152;
          pOBD->pInitFull = epd29r_init_sequence_152;
          pOBD->x_offset = 1;
      } else if (iType == EPD154R_152x152){
          pOBD->native_width = pOBD->width = 152;
          pOBD->native_height = pOBD->height = 152;
          pOBD->pInitFull = epd29r_init_sequence_152;
      } else if (iType == EPD213R2_122x250) {
          pOBD->native_width = pOBD->width = 122;
          pOBD->native_height = pOBD->height = 250;
          pOBD->pInitFull = epd29r_init_sequence;
          pOBD->x_offset = 1;
      } else {
          pOBD->native_width = pOBD->width = 400;
          pOBD->native_height = pOBD->height = 300;
          pOBD->pInitFull = epd42r_init_sequence;
      }
      pOBD->type = EPD29R_128x296; // use the rest of the same code
      pOBD->iTimeout = 25000; // 3-color need a longer timeout (25 seconds)
      pOBD->iFlags |= OBD_3COLOR;
      pOBD->chip_type = OBD_CHIP_SSD16xx;
      pOBD->can_flip = 0;
      pOBD->busy_idle = LOW;
      EPDSendCMDSequence(pOBD, pOBD->pInitFull);
      return;
  }
  else if (iType == EPD102_80x128) // not working yet
  {
      pOBD->native_width = pOBD->width = 80;
      pOBD->native_height = pOBD->height = 128;
      pOBD->can_flip = 0; // flip display commands don't exist
      pOBD->busy_idle = HIGH;
      pOBD->iFlags |= OBD_CS_EVERY_BYTE;
      return;
  }
  else if (iType == EPD154_152x152) // GDEW0154M10
  {
      pOBD->native_width = pOBD->width = 152;
      pOBD->native_height = pOBD->height = 152;
      pOBD->can_flip = 0; // flip display commands don't exist
      pOBD->busy_idle = HIGH;
      pOBD->chip_type = OBD_CHIP_UC8151;
      pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
//      pOBD->iFlags |= OBD_CS_EVERY_BYTE;
      pOBD->pInitFull = epd154b_init_sequence_full;
      pOBD->pInitFast = epd154b_init_sequence_fast;
      EPDSendCMDSequence(pOBD, pOBD->pInitFull);
      return;
  }
  else if (iType == EPD154_200x200)
  {
      pOBD->native_width = pOBD->width = 200;
      pOBD->native_height = pOBD->height = 200;
      pOBD->can_flip = 0; // flip display commands don't exist
      pOBD->busy_idle = LOW;
      pOBD->chip_type = OBD_CHIP_SSD16xx;
      pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
      pOBD->pInitFull = epd154_init_sequence_full;
      return;
  }
  else if (iType == EPD29_128x296 || iType == EPD213B_104x212 || iType == EPD37_240x416)
  {
      pOBD->iFlags |= OBD_HAS_FAST_UPDATE;
      pOBD->chip_type = OBD_CHIP_UC8151;
      pOBD->can_flip = 0; // flip display commands don't exist
      if (iType == EPD37_240x416) {
          pOBD->native_width = pOBD->width = 240;
          pOBD->native_height = pOBD->height = 416;
          pOBD->busy_idle = HIGH;
          pOBD->pInitFull = epd37_init_sequence_full;
          EPDSendCMDSequence(pOBD, epd37_init_sequence_full);
          pOBD->type = EPD29_128x296; // tell it to use this config
      } else if (iType == EPD29_128x296) {
          pOBD->native_width = pOBD->width = 128;
          pOBD->native_height = pOBD->height = 296;
          pOBD->busy_idle = HIGH;
          pOBD->pInitFull = epd29_init_sequence_full;
          EPDSendCMDSequence(pOBD, pOBD->pInitFull);
      } else {
          pOBD->native_width = pOBD->width = 104;
          pOBD->native_height = pOBD->height = 212;
          pOBD->busy_idle = LOW;
          pOBD->type = EPD29_128x296; // tell it to use this config
          EPD29_Init(pOBD);
      }
      return;
  } else if (iType == EPD47_540x960) {
      pOBD->native_width = pOBD->width = 540;
      pOBD->native_height = pOBD->height = 960;
      pOBD->can_flip = 0; // flip display commands don't exist
      pOBD->busy_idle = HIGH;
      return;
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
  else if (iType < LCD_UC1701) // 128x64 and 64x32
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
  if (iType == LCD_ST7302)
  {
 //     uint8_t *s = (uint8_t *)st7302_hpm_init;
      const uint8_t *s = (uint8_t *)st7302_wenting; //st7302_lpm_init;
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
  } /* ST7302 */
#endif // !WIMPY_MCU
  if (iType == LCD_UC1701 || iType == LCD_HX1230)
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
   if (pOBD->type == LCD_UC1701 || pOBD->type == LCD_UC1609)
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
static void EPDWaitBusy(OBDISP *pOBD, int bQuick)
{
#ifndef MEMORY_ONLY
int iTimeout = 0;
int iEndTime = (int)pOBD->iTimeout;
    
    if (bQuick)
        iEndTime /= 5; // 1/5th for short waits such as after Power-on
    
    if (pOBD->iLEDPin == 0xff || pOBD->iLEDPin == -1) {
        _delay(iEndTime); // no busy pin, wait fixed amount
        return;
    }
  while (1) {
//      if (pOBD->type == EPD29_128x296) {
//          obdWriteCommand(pOBD, UC8151_FLG);
//      }
     if (digitalRead(pOBD->iLEDPin) == pOBD->busy_idle)
         break;
     iTimeout += (iEndTime/4);
     if (iTimeout > iEndTime)
         break; // DEBUG - timeout
     _delay(iEndTime/4);
  }
    if (iTimeout > iEndTime) {
#if defined( ARDUINO ) && !defined(WIMPY_MCU)
        Serial.println("EPDWaitBusy() timed out");
#endif
#if defined(_LINUX_)
        printf("EPDWaitBusy() timed out\n");
#endif
    }
#endif // MEMORY_ONLY
} /* EPDWaitBusy() */

void EPD42_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
uint8_t ucTemp[8];
    
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    obdWriteCommand(pOBD, UC8151_PWR); // POWER SETTING
    ucTemp[0] = 0x03;   // VDS_EN, VDG_EN internal
    ucTemp[1] = 0x00;   // VCOM_HV, VGHL_LV=16V
    ucTemp[2] = 0x2b;   // VDH=11V
    ucTemp[3] = 0x2b;   // VDL=11V
    ucTemp[4] = 0xff;   // VDHR
    RawWriteData(pOBD, ucTemp, 5);
    obdWriteCommand(pOBD, UC8151_BTST); // boost soft start
    ucTemp[0] = 0x17;   // A
    ucTemp[1] = 0x17;   // B
    ucTemp[2] = 0x17;   // C
    RawWriteData(pOBD, ucTemp, 3);
    EPD_CMD2(pOBD, UC8151_PSR, 0x3f); // panel setting
    EPD_CMD2(pOBD, UC8151_PLL, 0x3a); // PLL setting 100HZ   29 150Hz 39 200HZ 31 171HZ
    obdWriteCommand(pOBD, UC8151_TRES); // resolution setting
    ucTemp[0] = (uint8_t)(pOBD->native_width >> 8);
    ucTemp[1] = (uint8_t)(pOBD->native_width);
    ucTemp[2] = (uint8_t)(pOBD->native_height >> 8);
    ucTemp[3] = (uint8_t)(pOBD->native_height);
    RawWriteData(pOBD, ucTemp, 4);
    EPD_CMD2(pOBD, UC8151_VDCS, 0x12); // vcom_DC setting = -0.1 + 18 * -0.05 = -1.0V from OTP, slightly better
    EPD_CMD2(pOBD, UC8151_CDI, 0xd7); // VCOM AND DATA INTERVAL SETTING
  //  ucTemp[0] = 0xd7;  // border floating to avoid flashing
    obdWriteCommand(pOBD, UC8151_PON);
    EPDWaitBusy(pOBD, 1); // power on
    if (bPartial) {
        EPDSendCMDSequence(pOBD, epd42_init_sequence_partial);
    } else {
        EPDSendCMDSequence(pOBD, epd29_init_sequence_full);
    }
} /* EPD42_Begin() */

void EPD27_SetLut(OBDISP *pOBD, int bFast)
{
    if (bFast) {
        obdWriteCommand(pOBD, UC8151_LUT_VCOM); //vcom
        RawWriteData(pOBD, (uint8_t *)lut_fast_vcom_dc, 44);
       
        obdWriteCommand(pOBD, UC8151_LUT_WW);
        RawWriteData(pOBD, (uint8_t *)lut_fast_ww, 42);
       
        obdWriteCommand(pOBD, UC8151_LUT_BW);
        RawWriteData(pOBD, (uint8_t *)lut_fast_bw, 42);

        obdWriteCommand(pOBD, UC8151_LUT_WB);
        RawWriteData(pOBD, (uint8_t *)lut_fast_bb, 42);

        obdWriteCommand(pOBD, UC8151_LUT_BB);
        RawWriteData(pOBD, (uint8_t *)lut_fast_wb, 42);
    } else {
        obdWriteCommand(pOBD, UC8151_LUT_VCOM); //vcom
        RawWriteData(pOBD, (uint8_t *)lut_vcom_dc, 44);
       
        obdWriteCommand(pOBD, UC8151_LUT_WW);
        RawWriteData(pOBD, (uint8_t *)lut_ww, 42);
       
        obdWriteCommand(pOBD, UC8151_LUT_BW);
        RawWriteData(pOBD, (uint8_t *)lut_bw, 42);

        obdWriteCommand(pOBD, UC8151_LUT_WB);
        RawWriteData(pOBD, (uint8_t *)lut_bb, 42);

        obdWriteCommand(pOBD, UC8151_LUT_BB);
        RawWriteData(pOBD, (uint8_t *)lut_wb, 42);
    }
}

void EPD27_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
uint8_t ucTemp[8];
    
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)bPartial;
//    obdWriteCommand(pOBD, UC8151_PON);
//    EPDWaitBusy(pOBD);
    obdWriteCommand(pOBD, UC8151_PWR); // POWER SETTING
    ucTemp[0] = 0x03;   // VDS_EN, VDG_EN internal
    ucTemp[1] = 0x00;   // VCOM_HV, VGHL_LV=16V
    ucTemp[2] = 0x2b;   // VDH=11V
    ucTemp[3] = 0x2b;   // VDL=11V
    ucTemp[4] = 0x09;   // VDHR
    RawWriteData(pOBD, ucTemp, 5);
    obdWriteCommand(pOBD, UC8151_BTST); // boost soft start
    ucTemp[0] = 0x07;   // A
    ucTemp[1] = 0x07;   // B
    ucTemp[2] = 0x17;   // C
    RawWriteData(pOBD, ucTemp, 3);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0x60;
    ucTemp[1] = 0xa5;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization (again)
    ucTemp[0] = 0x89;
    ucTemp[1] = 0xa5;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0x90;
    ucTemp[1] = 0x00;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0x93;
    ucTemp[1] = 0x2A;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0xa0;
    ucTemp[1] = 0xa5;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0xa1;
    ucTemp[1] = 0xa0;
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0xf8); // power optimization
    ucTemp[0] = 0x73;
    ucTemp[1] = 0x41;
    RawWriteData(pOBD, ucTemp, 2);
    EPD_CMD2(pOBD, 0x16, 0); // partial refresh off
    obdWriteCommand(pOBD, UC8151_PON); // power on
    EPDWaitBusy(pOBD, 1);
    EPD_CMD2(pOBD, UC8151_PSR, 0xaf); // panel setting
    EPD_CMD2(pOBD, UC8151_PLL, 0x3a); // PLL setting 100HZ   29 150Hz 39 200HZ 31 171HZ
    EPD_CMD2(pOBD, UC8151_VDCS, 0x12); // vcom_DC setting = -0.1 + 18 * -0.05 = -1.0V from OTP, slightly better
    _delay(2);
    
//    obdWriteCommand(pOBD, UC8151_TRES); // resolution setting
//    ucTemp[0] = (uint8_t)(pOBD->native_width >> 8);
//    ucTemp[1] = (uint8_t)(pOBD->native_width);
//    ucTemp[2] = (uint8_t)(pOBD->native_height >> 8);
//    ucTemp[3] = (uint8_t)(pOBD->native_height);
//    RawWriteData(pOBD, ucTemp, 4);
//    EPD_CMD2(pOBD, UC8151_CDI, 0x07); // VCOM AND DATA INTERVAL SETTING (white border)
    EPD27_SetLut(pOBD, 0); // fast mode not ready bPartial);
} /* EPD27_Begin() */

//
// Wake up the EPD controller
//
static void EPDWakeUp(OBDISP *pOBD)
{
    if (pOBD->iRSTPin != 0xff) {
        digitalWrite(pOBD->iRSTPin, LOW);
        _delay(10);
        digitalWrite(pOBD->iRSTPin, HIGH);
        _delay(200);
    }
    if (pOBD->chip_type == OBD_CHIP_UC8151) {
        obdWriteCommand(pOBD, UC8151_PON);
    } else {
        obdWriteCommand(pOBD, SSD1608_SW_RESET); // soft reset
    }
    EPDWaitBusy(pOBD, 1);
} /* EPDWakeUp() */

static void EPDSleep(OBDISP *pOBD)
{

    if (pOBD->chip_type == OBD_CHIP_SSD16xx) {
        if (pOBD->iRSTPin != 0xff)
            EPD_CMD2(pOBD, 0x10, 0x02); // deep sleep mode 2 (loses RAM, needs reset)
        else // no reset pin, put it into light sleep
            EPD_CMD2(pOBD, 0x10, 0x00);
        return;
    }
  EPD_CMD2(pOBD, UC8151_CDI, 0x17); // border floating
  obdWriteCommand(pOBD, UC8151_POF); // power off
  EPDWaitBusy(pOBD, 1);
  if (pOBD->iRSTPin != 0xff)
  {
    EPD_CMD2(pOBD, UC8151_DSLP, 0xa5); // deep sleep
  }
} /* EPDSleep() */
//
// Set the boundaries of the partial update area
// start/end x/y
//
void EPDSetPartialArea(OBDISP *pOBD, int sx, int sy, int ex, int ey)
{
uint8_t ucTemp[12];

  ucTemp[0] = 0x40; // data operation
  sx &= 0xFFF8; // byte boundary
  ex = (ex - 1) | 0x7; // byte boundary - 1
  obdWriteCommand(pOBD, 0x90); // partial window
  ucTemp[1] = (uint8_t)(sx / 256);
  ucTemp[2] = (uint8_t)(sx & 256);
  ucTemp[3] = (uint8_t)(ex / 256);
  ucTemp[4] = (uint8_t)(ex & 256);
  ucTemp[5] = (uint8_t)(sy / 256);
  ucTemp[6] = (uint8_t)(sy & 256);
  ucTemp[7] = (uint8_t)(ey / 256);
  ucTemp[8] = (uint8_t)(ey & 256);
  ucTemp[9] = 0x01; // gates scan inside and outside the partial area (don't see any difference)
  RawWrite(pOBD, ucTemp, 10);
  _delay(2);
  //IO.writeDataTransaction(0x00); // don't see any difference
  //return (7 + xe - x) / 8; // number of bytes to transfer
} /* EPDSetPartialArea() */

void EPD154_Finish(OBDISP *pOBD, int bPartial)
{
    if (bPartial) {
        EPD_CMD2(pOBD, SSD1608_DISP_CTRL2, 0xcf);
    } else {
        EPD_CMD2(pOBD, SSD1608_DISP_CTRL2, 0xc7);
    }
    obdWriteCommand(pOBD, SSD1608_MASTER_ACTIVATE);
    EPDWaitBusy(pOBD, 0);
} /* EPD154_Finish() */

void EPD29_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
uint8_t ucLine[8];
    
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    if (bPartial) {
        EPDSendCMDSequence(pOBD, epd29_init_sequence_partial);
        obdWriteCommand(pOBD, UC8151_PON); // power on
        obdWriteCommand(pOBD, UC8151_PTIN); // partial mode
        obdWriteCommand(pOBD, UC8151_PTL); // partial window
        ucLine[0] = 0; //y; X START
        ucLine[1] = pOBD->native_width-1; //(y + h - 1); X END
        ucLine[2] = 0; //(x >> 8); // Y START
        ucLine[3] = 0; //x & 0xff;
        ucLine[4] = (uint8_t)((pOBD->native_height-1) >> 8); //(x + w - 1) >> 8; Y END >> 8
        ucLine[5] = (uint8_t)((pOBD->native_height-1) & 0xff); //(x + w - 1) & 0xff;
        ucLine[6] = 0x28; // ?
        RawWriteData(pOBD, ucLine, 7); // boundaries
    } else {
        EPDSendCMDSequence(pOBD, epd29_init_sequence_full);
        obdWriteCommand(pOBD, UC8151_PTOU); // disable partial mode
    }
} /* EPD29_Begin() */

void EPD29_Finish(OBDISP *pOBD, int bPartial)
{
    (void)bPartial;
    obdWriteCommand(pOBD, UC8151_DSP); // stop data
    obdWriteCommand(pOBD, UC8151_DRF); // start display refresh
    _delay(10); // needed
    EPDWaitBusy(pOBD, 0);
} /* EPD29_Finish() */
//
// Special case for some e-ink displays
//
static int EPDDumpFast(OBDISP *pOBD, uint8_t *pBuffer, int x, int y, int w, int h)
{
    if (pBuffer == NULL)
      pBuffer = pOBD->ucScreen;
    if (pBuffer == NULL)
        return OBD_ERROR_NO_MEMORY;
    
    EPDWakeUp(pOBD);
    
    if (pOBD->type == EPD29R_128x296 || pOBD->type == EPD154R_152x152 || pOBD->type == EPD42R_400x300) {
        if (pOBD->iFlags & OBD_FULLUPDATE) {
            // Kludge alert - fool it into going into B&W mode to allow partial updates. Need to combine the black+red pixels and send them to the ALTRAM first
            uint8_t *s = pOBD->ucScreen;
            int iOffset = ((pOBD->native_width+7)/8) * pOBD->native_height; // full buffer size
            for (int i=0; i<iOffset; i++) {
                s[i] |= s[i+iOffset]; // combine red+black pixels
            }
            // do a fake full update first
            EPD213_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
#ifndef WIMPY_MCU
            EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, SSD1608_WRITE_ALTRAM, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send image to current and previous buffers
#endif
            return OBD_SUCCESS;
        }
        EPD213_Begin(pOBD, x, y, w, h, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, x, y, w, h, 0);
#endif
        EPD154_Finish(pOBD, true);
    }
    if (pOBD->type == EPD29_128x296) {
        EPD29_Begin(pOBD, x, y, w, h, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, UC8151_DTM2, NULL, x, y, w, h, 0);
#endif
        EPD29_Finish(pOBD, true);
#ifndef WIMPY_MCU
        // The new becomes the old pixels for next time through
        EPD29_Begin(pOBD, x, y, w, h, true);
        EPDWriteImage(pOBD, UC8151_DTM1, NULL, x, y, w, h, 0);
        EPDWriteImage(pOBD, UC8151_DTM2, NULL, x, y, w, h, 0);
#endif
    } else if (pOBD->type == EPD213_104x212 || pOBD->type == EPD213_122x250) {
        EPD213_Begin(pOBD, x, y, w, h, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, x, y, w, h, 0);
#endif
        EPD154_Finish(pOBD, true);
    } else if (pOBD->type == EPD154_200x200) {
        EPD154_Begin(pOBD, x, y, w, h, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, x, y, w, h, 0);
#endif
        EPD154_Finish(pOBD, true);
    } else if (pOBD->type == EPD293_128x296) {
        EPD293_Begin(pOBD, x, y, w, h, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, 0x24, NULL, 0, 0, pOBD->width, pOBD->height, 0);
#endif
        EPD_CMD2(pOBD, 0x22, 0xcf);
        obdWriteCommand(pOBD, 0x20); // refresh
        EPDWaitBusy(pOBD, 0);
     //   delay(5000);
    } else if (pOBD->type == EPD154_152x152) {
        EPDSendCMDSequence(pOBD, pOBD->pInitFast);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0);
        }
#endif
        obdWriteCommand(pOBD, UC8151_DRF);
    }
    if (pOBD->type == EPD42_400x300) {
        EPD42_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0);
#endif
        obdWriteCommand(pOBD, UC8151_DRF);
    }
    if (pOBD->type == EPD27_176x264) {
        EPD27_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, true);
#ifndef WIMPY_MCU
        EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0);
#endif
        obdWriteCommand(pOBD, UC8151_DRF);
    }
    EPDSleep(pOBD);
    return OBD_SUCCESS;
} /* EPDDumpFast() */

void EPD_CMD2(OBDISP *pOBD, uint8_t cmd, uint8_t param)
{
	obdWriteCommand(pOBD, cmd);
	RawWriteData(pOBD, &param, 1);
} /* EPD_CMD2() */

void EPD213_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
uint8_t ucLine[8];

    // set luts
    obdWriteCommand(pOBD, SSD1608_WRITE_LUT);
//    if (pOBD->type == EPD213_104x212) {
//       RawWriteData(pOBD, (uint8_t *)epd213_lut, sizeof(epd213_lut));
//    } else { // 122x250
    if (bPartial) {
//        obdWriteCommand(pOBD, SSD1608_WRITE_LUT);
        memcpy_P(u8Cache, epd213_lut2_partial, sizeof(epd213_lut2_partial));
        RawWriteData(pOBD, u8Cache, sizeof(epd213_lut2_partial));
    }  else {
        memcpy_P(u8Cache, epd213_lut2_full, sizeof(epd213_lut2_full));
            RawWriteData(pOBD, u8Cache, sizeof(epd213_lut2_full));
    }

    obdWriteCommand(pOBD, 0x44); // set RAM X start/end
    if (pOBD->iOrientation == 0 || pOBD->iOrientation == 180) {
        ucLine[0] = x/8;
        ucLine[1] = (x+w-1)/8;
    } else { // 90/270
        ucLine[0] = y/8;
        ucLine[1] = (y+h-1)/8;
    }
    RawWriteData(pOBD, ucLine, 2);
    obdWriteCommand(pOBD, 0x45); // set RAM Y start/end
    if (pOBD->iOrientation == 0 || pOBD->iOrientation == 180) {
        ucLine[0] = (uint8_t)y;
        ucLine[1] = (uint8_t)(y>>8);
        ucLine[2] = (uint8_t)(y+h-1);
        ucLine[3] = (uint8_t)((y+h-1)>>8);
    } else { // 90/270
        ucLine[0] = (uint8_t)x;
        ucLine[1] = (uint8_t)(x>>8);
        ucLine[2] = (uint8_t)(x+w-1);
        ucLine[3] = (uint8_t)((x+w-1)>>8);
    }
    RawWriteData(pOBD, ucLine, 4);
    EPD_CMD2(pOBD, 0x4e, 0x00); // set RAM X pointer start
    ucLine[0] = 0x00;
    ucLine[1] = 0x00;
    obdWriteCommand(pOBD, 0x4f); // set RAM Y pointer start
    RawWriteData(pOBD, ucLine, 2);
    if (bPartial) {
        EPD_CMD2(pOBD, SSD1608_WRITE_VCOM, 0x26); // VCOM
        EPDWaitBusy(pOBD, 1);
        obdWriteCommand(pOBD, 0x37); // ?
        memset(ucLine, 0, 7);
        ucLine[4] = 0x40;
        RawWriteData(pOBD, ucLine, 7);
        
        EPD_CMD2(pOBD, 0x22, 0xc0);
        obdWriteCommand(pOBD, 0x20);
        EPDWaitBusy(pOBD, 1);
        EPD_CMD2(pOBD, 0x3c, 0x01); // border waveform
    } else {
        EPD_CMD2(pOBD, SSD1608_WRITE_VCOM, 0x55); // VCOM
        EPDWaitBusy(pOBD, 1);
    }
} /* EPD213_Begin() */

void EPD154_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
    obdWriteCommand(pOBD, SSD1608_WRITE_LUT);
    if (bPartial) {
        // send partial update lut
        memcpy_P(u8Cache, lut154_partial_update, sizeof(lut154_partial_update));
        RawWriteData(pOBD, u8Cache, sizeof(lut154_partial_update));
        EPD_CMD2(pOBD, 0x3f, 0x2); // ?
        EPD_CMD2(pOBD, SSD1608_GATE_VOLTAGE, 0x17);
        u8Cache[0] = 0x41;
        u8Cache[1] = 0xb0;
        u8Cache[2] = 0x32;
        obdWriteCommand(pOBD, SSD1608_SOURCE_VOLTAGE);
        RawWriteData(pOBD, u8Cache, 3);
        EPD_CMD2(pOBD, SSD1608_WRITE_VCOM, 0x28); // VCOM
        
        obdWriteCommand(pOBD, 0x37); // ?
        memset(u8Cache, 0, 10);
        u8Cache[5] = 0x40;
        RawWriteData(pOBD, u8Cache, 10);
        
        EPD_CMD2(pOBD, 0x3c, 0x80); // VCOM?
        EPD_CMD2(pOBD, 0x22, 0xc0);
        obdWriteCommand(pOBD, 0x20);
        EPDWaitBusy(pOBD, 0);
    } else {
        // send full update lut
        memcpy_P(u8Cache, lut154_full_update, sizeof(lut154_full_update));
        RawWriteData(pOBD, u8Cache, sizeof(lut154_full_update));
        EPD_CMD2(pOBD, 0x3f, 0x22); // ?
        EPD_CMD2(pOBD, SSD1608_GATE_VOLTAGE, 0x17);
        u8Cache[0] = 0x41;
        u8Cache[1] = 0x00;
        u8Cache[2] = 0x32;
        obdWriteCommand(pOBD, SSD1608_SOURCE_VOLTAGE);
        RawWriteData(pOBD, u8Cache, 3);
        EPD_CMD2(pOBD, SSD1608_WRITE_VCOM, 0x20); // VCOM
    }
    if (pOBD->iFlags & OBD_FLIPV)
        EPD_CMD2(pOBD, SSD1608_DATA_MODE, 0x1);
    else
        EPD_CMD2(pOBD, SSD1608_DATA_MODE, 0x3);
    obdWriteCommand(pOBD, SSD1608_SET_RAMXPOS); // set RAM X start/end
    u8Cache[0] = x/8;
    u8Cache[1] = (x+w-1)/8;
    RawWriteData(pOBD, u8Cache, 2);
    obdWriteCommand(pOBD, SSD1608_SET_RAMYPOS); // set RAM Y start/end
    if (pOBD->iFlags & OBD_FLIPV) {
        u8Cache[0] = (uint8_t)(y+h-1);
        u8Cache[1] = (uint8_t)((y+h-1)>>8);
        u8Cache[2] = (uint8_t)y;
        u8Cache[3] = (uint8_t)(y>>8);
    } else {
        u8Cache[0] = (uint8_t)y;
        u8Cache[1] = (uint8_t)(y>>8);
        u8Cache[2] = (uint8_t)(y+h-1);
        u8Cache[3] = (uint8_t)((y+h-1)>>8);
    }
    RawWriteData(pOBD, u8Cache, 4);
    EPD_CMD2(pOBD, SSD1608_SET_RAMXCOUNT, 0); // set RAM X pointer start
    obdWriteCommand(pOBD, SSD1608_SET_RAMYCOUNT); // set RAM Y pointer start
    if (pOBD->iFlags & OBD_FLIPV) {
        u8Cache[0] = y+h-1;
    } else {
        u8Cache[0] = 0x00;
    }
    u8Cache[1] = 0x00;
    RawWriteData(pOBD, u8Cache, 2);
} /* EPD154_Begin() */

void EPD8951_REG(OBDISP *pOBD, uint16_t addr, uint16_t data)
{
uint8_t ucTemp[4];
    
    EPDWaitBusy(pOBD, 1);
    ucTemp[0] = 0x60; // cmd intro 6000h
    ucTemp[1] = 0x00;
    ucTemp[2] = 0x00; // reg write 0011h
    ucTemp[3] = 0x11;
    RawWriteData(pOBD, ucTemp, 4);
    EPDWaitBusy(pOBD, 1);
    ucTemp[0] = ucTemp[1] = 0; // 0000 data intro
    ucTemp[2] = (uint8_t)(addr >> 8);
    ucTemp[3] = (uint8_t)(addr & 0xff);
    RawWriteData(pOBD, ucTemp, 4);
    ucTemp[2] = (uint8_t)(data >> 8);
    ucTemp[3] = (uint8_t)(data & 0xff);
    RawWriteData(pOBD, ucTemp, 4);

} /* EPD8951_REG() */

void EPD8951_ARGS(OBDISP *pOBD, uint16_t u16CMD, uint16_t *pArgs, int iCount)
{
    uint8_t ucTemp[4];
    EPDWaitBusy(pOBD, 1);
    ucTemp[0] = 0x60;
    ucTemp[1] = 0x00; // 6000 cmd
    ucTemp[2] = (uint8_t)(u16CMD >> 8);
    ucTemp[3] = (uint8_t)(u16CMD & 0xff);
    RawWriteData(pOBD, ucTemp, 4);
    
    for (int i=0; i<iCount; i++) {
        EPDWaitBusy(pOBD, 1);
        ucTemp[0] = 0x00;
        ucTemp[1] = 0x00; // 0000 data
        ucTemp[2] = (uint8_t)(pArgs[i] >> 8);
        ucTemp[3] = (uint8_t)(pArgs[i] & 0xff);
        RawWriteData(pOBD, ucTemp, 4);
    }
} /* EPD8951_ARGS() */

void EPD8951_CMD(OBDISP *pOBD, int iCMD)
{
uint8_t ucTemp[4];
    
    EPDWaitBusy(pOBD, 1);
    ucTemp[0] = 0x60;
    ucTemp[1] = 0x00; // cmd prefix = 6000h
    ucTemp[2] = (uint8_t)(iCMD >> 8);
    ucTemp[3] = (uint8_t)(iCMD & 0xff);
    RawWriteData(pOBD, ucTemp, 4);
} /* EPD8951_CMD() */

void EPD8951_WORD(OBDISP *pOBD, uint16_t u16Word)
{
uint8_t ucTemp[4];
    
    EPDWaitBusy(pOBD, 1);
    ucTemp[0] = 0x00;
    ucTemp[1] = 0x00; // cmd prefix = 6000h
    ucTemp[2] = (uint8_t)(u16Word >> 8);
    ucTemp[3] = (uint8_t)(u16Word & 0xff);
    RawWriteData(pOBD, ucTemp, 4);
} /* EPD8951_WORD() */

static void EPDFill(OBDISP *pOBD, uint8_t ucCMD, uint8_t ucPattern)
{
int iSize;

    if (ucCMD) {
        obdWriteCommand(pOBD, ucCMD); // start write
    }
    if (pOBD->type == EPD579_792x272) {
        iSize = 400*(272>>3);
    } else {
        iSize = ((pOBD->native_width+7)>>3) * pOBD->native_height;
    }
    for (int i=0; i<iSize; i++) {
        RawWriteData(pOBD, &ucPattern, 1); // slow to go one byte at a time, but not a critical time issue for this feature and we need to support a "zero RAM" environment
    }
} /* EPDFill() */

//
// Write EPD Image data
//
#ifndef WIMPY_MCU
static void EPDWriteImage(OBDISP *pOBD, uint8_t ucCMD, uint8_t *pBits, int x, int y, int w, int h, int bRed)
{
int tx, ty;
uint8_t *s, *d, ucSrcMask, ucDstMask, uc;
uint8_t *pBuffer, ucInvert = 0;
int iPitch;
    
    if (pBits == NULL) { // use back buffer
        pBuffer = pOBD->ucScreen;
        iPitch = pOBD->width;
    } else {
        // assume the passed buffer is only the size and width of the requested write
        pBuffer = pBits;
        iPitch = w;
    }
    if (bRed) {// use second bit plane
        pBuffer += pOBD->width * ((pOBD->height+7)/8);
        ucInvert = 0xff; // red logic is inverted
    }
    if (ucCMD) {
        obdWriteCommand(pOBD, ucCMD); // start write
    }
  // Convert the bit direction and write the data to the EPD
  u8Cache[0] = u8Cache[1] = 0; // prefix for IT8951
  if (pOBD->iOrientation == 180) {
      for (ty=y+h-1; ty>=y; ty--) {
     ucSrcMask = 1 << (ty & 7);
     ucDstMask = 0x80;
     uc = 0xff;
     d = u8Cache+2;
     s = &pBuffer[(ty>>3) * iPitch];
     for (tx=x+w-1; tx>=x; tx--) {
            if (s[tx] & ucSrcMask)
           uc &= ~ucDstMask;
        ucDstMask >>= 1;
        if (ucDstMask == 0) {
           *d++ = (uc ^ ucInvert);
           uc = 0xff;
           ucDstMask = 0x80;
        }
     } // for tx
     *d++ = uc; // store final partial byte
     if (pOBD->type == EPD47_540x960)
         RawWriteData(pOBD, u8Cache, 2 + ((w+7)/8));
     else
         RawWriteData(pOBD, u8Cache+2, (w+7)/8);
      } // for ty
  } else if (pOBD->iOrientation == 0) {
      for (ty=y; ty<y+h; ty++) {
         ucSrcMask = 1 << (ty & 7);
         ucDstMask = 0x80;
         uc = 0xff;
         d = u8Cache+2;
         s = &pBuffer[(ty >> 3) * iPitch];
         for (tx=x; tx<x+w; tx++) {
            if (s[tx] & ucSrcMask) // src pixel set
               uc &= ~ucDstMask;
            ucDstMask >>= 1;
            if (ucDstMask == 0) { // completed byte
               *d++ = (uc ^ ucInvert);
               uc = 0xff;
               ucDstMask = 0x80;
            }
         } // for tx
        *d++ = (uc ^ ucInvert); // store final partial byte
        if (pOBD->type == EPD47_540x960)
            RawWriteData(pOBD, u8Cache, 2+((w+7)/8));
        else
            RawWriteData(pOBD, u8Cache+2, (w+7)/8);
      } // for ty
  } else if (pOBD->iOrientation == 90) {
      for (tx=x; tx<x+w; tx++) {
         d = u8Cache+2;
     // need to pick up and reassemble every pixel
     ucDstMask = 0x80;
     uc = 0xff;
     ucSrcMask = 1 << ((y+h-1) & 7);
     for (ty=y+h-1; ty>=y; ty--) {
            s = &pBuffer[tx + (ty>>3) * iPitch];
        if (s[0] & ucSrcMask) uc &= ~ucDstMask;
        ucDstMask >>= 1;
        ucSrcMask >>= 1;
            if (ucDstMask == 0) {
               *d++ = (uc ^ ucInvert);
           ucDstMask = 0x80;
           uc = 0xff;
        }
        if (ucSrcMask == 0)
           ucSrcMask = 0x80; // bottom up
     } // for ty
      *d++ = (uc ^ ucInvert); // store final partial byte
          if (pOBD->type == EPD47_540x960)
              RawWriteData(pOBD, u8Cache, 2+((h+7)/8));
         else
             RawWriteData(pOBD, u8Cache+2, (h+7)/8);
      } // for tx
  } else if (pOBD->iOrientation == 270) {
      for (tx=x+w-1; tx>=x; tx--) {
      d = u8Cache+2;
      // reassemble every pixel
      ucDstMask = 0x80;
      uc = 0xff;
      ucSrcMask = 1 << (y & 7);
      for (ty=y; ty<y+h; ty++) {
          s = &pBuffer[tx + (ty>>3) * iPitch];
          if (s[0] & ucSrcMask) uc &= ~ucDstMask;
          ucDstMask >>= 1;
          ucSrcMask <<= 1;
          if (ucDstMask == 0) {
         *d++ = (uc ^ ucInvert);
         ucDstMask = 0x80;
         uc = 0xff;
          }
          if (ucSrcMask == 0)
         ucSrcMask = 1;
      } // for ty
      *d++ = (uc ^ ucInvert); // store final partial byte
      if (pOBD->type == EPD47_540x960)
          RawWriteData(pOBD, u8Cache, 2+((h+7)/8));
      else
          RawWriteData(pOBD, u8Cache+2, (h+7)/8);
      } // for x
  } // 270
} /* EPDWriteImage() */
//
// Write EPD Image data
//
static void EPDWriteImage4bpp(OBDISP *pOBD, uint8_t ucCMD, int x, int y, int w, int h)
{
uint8_t ucLine[324]; // send the data one line at a time
int tx, ty, iRedOff;
uint8_t *s, *d, ucSrcMask, uc;
uint8_t *pBuffer;
    
    pBuffer = pOBD->ucScreen;
    iRedOff = pOBD->width * ((pOBD->height+7)/8);
    if (ucCMD) {
        obdWriteCommand(pOBD, ucCMD); // start write
    }
  // Convert the bit direction and write the data to the EPD
    // This particular controller has 4 bits per pixel where 0=black, 3=white, 4=red
    // this wastes 50% of the time transmitting bloated info (only need 2 bits)
  if (pOBD->iOrientation == 180) {
      for (ty=y+h-1; ty>=y; ty--) {
     ucSrcMask = 1 << (ty & 7);
     d = ucLine;
     s = &pBuffer[(ty>>3) * pOBD->width];
     for (tx=x+w-1; tx>=x; tx-=2) {
         uc = 0x33; // start with white
            if (s[tx] & ucSrcMask) // black
                uc = 0x03;
         if (s[tx+iRedOff] & ucSrcMask) // red
             uc = 0x43;
         if (s[tx-1] & ucSrcMask) {// src pixel = black
             uc &= 0xf0;
         }
         if (s[tx-1+iRedOff] & ucSrcMask) { // red
              uc &= 0xf0; uc |= 0x4;
         }
           *d++ = uc; // store 2 pixels
     } // for tx
      RawWriteData(pOBD, ucLine, w/2);
      } // for ty
  } else if (pOBD->iOrientation == 0) {
      for (ty=y; ty<y+h; ty++) {
         ucSrcMask = 1 << (ty & 7);
         d = ucLine;
         s = &pBuffer[(ty >> 3) * pOBD->width];
         for (tx=x; tx<x+w; tx+=2) {
             uc = 0x33; // start with white
             if (s[tx] & ucSrcMask) {// src pixel = black
                uc = 0x03;
             }
             if (s[tx+iRedOff] & ucSrcMask) { // red
                 uc = 0x43;
             }
             if (s[tx+1] & ucSrcMask) {// src pixel = black
                 uc &= 0xf0;
             }
             if (s[tx+1+iRedOff] & ucSrcMask) { // red
                  uc &= 0xf0; uc |= 0x4;
             }
               *d++ = uc; // store 2 pixels
         } // for tx
        RawWriteData(pOBD, ucLine, w/2);
      } // for ty
  } else if (pOBD->iOrientation == 90) {
      for (tx=x; tx<x+w; tx++) {
         d = ucLine;
     ucSrcMask = 1 << ((y+h-1) & 7);
     for (ty=y+h-1; ty>=y; ty-=2) {
            s = &pBuffer[tx + (ty>>3) * pOBD->width];
         uc = 0x33;
        if (s[0] & ucSrcMask)
            uc = 0x03; // black
         if (s[iRedOff] & ucSrcMask)
             uc = 0x43; // red
         ucSrcMask >>= 1;
         if (s[0] & ucSrcMask) {// src pixel = black
             uc &= 0xf0;
         }
         if (s[iRedOff] & ucSrcMask) { // red
              uc &= 0xf0; uc |= 0x4;
         }
         ucSrcMask >>= 1;
         *d++ = uc; // store 2 pixels
        if (ucSrcMask == 0)
           ucSrcMask = 0x80; // bottom up
     } // for ty
        RawWriteData(pOBD, ucLine, h/2);
      } // for tx
  } else if (pOBD->iOrientation == 270) {
      for (tx=x+w-1; tx>=x; tx--) {
      d = ucLine;
      ucSrcMask = 1 << (y & 7);
      for (ty=y; ty<y+h; ty+=2) {
          s = &pBuffer[tx + (ty>>3) * pOBD->width];
          uc = 0x33; // start with white
          if (s[0] & ucSrcMask)
              uc = 0x03; // black
          if (s[iRedOff] & ucSrcMask)
              uc = 0x43; // red
          ucSrcMask <<= 1;
          if (s[0] & ucSrcMask) {// src pixel = black
              uc &= 0xf0;
          }
          if (s[iRedOff] & ucSrcMask) { // red
               uc &= 0xf0; uc |= 0x4;
          }
          ucSrcMask <<= 1;
         *d++ = uc; // store 2 pixels
          if (ucSrcMask == 0)
         ucSrcMask = 1;
      } // for ty
        RawWriteData(pOBD, ucLine, h/2);
      } // for x
  } // 270
} /* EPDWriteImage4bpp() */
#endif // !WIMPY_MCU
const uint8_t epd293_init_sequence_part[] = 
{
//    0x01, SSD1608_SW_RESET,
//    BUSY_WAIT,
    0x02, 0x74, 0x54,
    0x02, 0x7e, 0x3b,
    0x03, 0x2b, 0x04, 0x63,
    0x05, 0x0c, 0x8f, 0x8f, 0x8f, 0x3f,
    0x04, 0x01, 0x27, 0x01, 0x00,
    0x02, 0x11, 0x03,
    0x03, 0x44, 0x00, 0x0f,
    0x05, 0x45, 0x00, 0x00, 0x27, 0x01,
    0x02, 0x3c, 0x01,
    0x02, 0x18, 0x80,
    0x02, 0x22, 0xb1,
    0x01, 0x20,
    BUSY_WAIT,
    0x01, 0x1b,
    0x02, 0x0d, 0xff,
    0x02, 0x4e, 0x00,
    0x03, 0x4f, 0x00, 0x00,
    0x00 // end of table
}; /* epd293_init_sequence_full[] */

const uint8_t epd293_lut[] =
{
    0x22,  0x11, 0x10, 0x00, 0x10, 0x00, 0x00, 0x11, 0x88, 0x80, 0x80, 0x80, 0x00, 0x00, 0x6A, 0x9B,
    0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x6A, 0x9B, 0x9B, 0x9B, 0x9B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0x18, 0x04, 0x16, 0x01, 0x0A, 0x0A, 0x0A, 0x0A, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x08, 0x3C, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void EPD293_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    if (bPartial) {
        EPDSendCMDSequence(pOBD, epd293_init_sequence_part);
        // send LUT
        obdWriteCommand(pOBD, 0x32);
        memcpy_P(u8Cache, epd293_lut_partial, sizeof(epd293_lut_partial));
        RawWriteData(pOBD, u8Cache, sizeof(epd293_lut_partial));
    } else {
        EPDSendCMDSequence(pOBD, epd293_init_sequence_full);
    }
} /* EPD293_Begin() */

void EPD47_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    EPD8951_CMD(pOBD, 0x0001); // IT8951_TCON_SYS_RUN
    EPD8951_REG(pOBD, 0x0004, 0x0001); // enable pack write
    // set vcom to -2.3v
    EPD8951_CMD(pOBD, 0x0039);
    EPD8951_WORD(pOBD, 0x0001);
    EPD8951_WORD(pOBD, 2300);
    if (bPartial) {
        
    } else {
        uint16_t u16Args[5];
        // set target memory area
        EPD8951_REG(pOBD, 0x000a, 0x0012); // _dev_memaddr_h
        EPD8951_REG(pOBD, 0x0008, 0x36e0); // _dev_memaddr_l
        // set image area
        u16Args[0] = 0; // little endian, 2bpp, no rotation
        u16Args[1] = 0; // start x
        u16Args[2] = 0; // start y
        u16Args[3] = pOBD->native_width;
        u16Args[4] = pOBD->native_height;
        EPD8951_ARGS(pOBD, 0x0021, u16Args, 5); // IT8951_TCON_LD_IMG_AREA
        EPD8951_CMD(pOBD, 0x20); // IT8951_TCON_LD_IMG
        EPD8951_WORD(pOBD, 0); // default settings (endian, format, rotation)
    }
} /* EPD47_Begin() */

#ifdef FUTURE
void EPD102_Begin(OBDISP *pOBD, int x, int y, int w, int h, int bPartial)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
        if (bPartial) {
            EPDSendCMDSequence(pOBD, epd29_init_sequence_partial);
            obdWriteCommand(pOBD, UC8151_PON); // power on
            obdWriteCommand(pOBD, UC8151_PTIN); // partial mode
            obdWriteCommand(pOBD, UC8151_PTL); // partial window
            u8Cache[0] = 0; //y; X START
            u8Cache[1] = 127; //(y + h - 1); X END
            u8Cache[2] = 0; //(x >> 8); // Y START
            u8Cache[3] = 0; //x & 0xff;
            u8Cache[4] = 1; //(x + w - 1) >> 8; Y END >> 8
            u8Cache[5] = 39; //(x + w - 1) & 0xff;
            u8Cache[6] = 0x28; // ?
            RawWriteData(pOBD, u8Cache, 7); // boundaries
        } else {
            EPDSendCMDSequence(pOBD, epd102_init_sequence_full);
            return;
            // send LUTs
            obdWriteCommand(pOBD, UC8151_LUT_WB);
            memcpy_P(u8Cache, epd102_lut_w_full, sizeof(epd102_lut_w_full));
            RawWriteData(pOBD, u8Cache, sizeof(epd102_lut_w_full));
            obdWriteCommand(pOBD, UC8151_LUT_BB);
            memcpy_P(u8Cache, epd102_lut_b_full, sizeof(epd102_lut_b_full));
            RawWriteData(pOBD, u8Cache, sizeof(epd102_lut_b_full));
      //      obdWriteCommand(pOBD, UC8151_PTOU); // disable partial mode
            obdWriteCommand(pOBD, UC8151_PON); // power on
            EPDWaitBusy(pOBD, 1);
        }
} /* EPD102_Begin() */
#endif // FUTURE
//
// Special case for e-ink displays
//
static int EPDDumpBuffer(OBDISP *pOBD, int bRefresh)
{
    pOBD->iFlags |= OBD_FULLUPDATE; // mark it for later
    EPDWakeUp(pOBD);
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
    // not enough RAM on AVR to use this type of EPD
    if (pOBD->type == EPD74R_640x384) {
        if (pOBD->native_width == 640) {
            EPDSendCMDSequence(pOBD, epd74r_init_sequence_full);
        } else {
            EPDSendCMDSequence(pOBD, epd583r_init_sequence_full);
        }
        if (pOBD->ucScreen) {
            EPDWriteImage4bpp(pOBD, 0x10, 0, 0, pOBD->width, pOBD->height);
        }
        if (bRefresh) {
            obdWriteCommand(pOBD, 0x12); // master activation
        }
        EPDWaitBusy(pOBD, 0);
        return OBD_SUCCESS;
    }
#endif // !WIMPY_MCU
    if (pOBD->type == EPD42R2_400x300) {
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
       // EPD27_SetLut(pOBD, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0x10, NULL, 0, 0, pOBD->width, pOBD->height, 0); // black/white plane
            EPDWriteImage(pOBD, 0x13, NULL, 0, 0, pOBD->width, pOBD->height, 1); // red plane
        }
#endif // !WIMPY_MCU
        if (bRefresh) {
            obdWriteCommand(pOBD, 0x12); // master activation
        }
    }
    if (pOBD->type == EPD213R_104x212_d) {
        EPDSendCMDSequence(pOBD, EPD213R_104x212_d_init_sequence_full);
        EPD27_SetLut(pOBD, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0x10, NULL, 0, 0, pOBD->width, pOBD->height, 0); // black/white plane
            EPDWriteImage(pOBD, 0x13, NULL, 0, 0, pOBD->width, pOBD->height, 1); // red plane
        }
#endif // !WIMPY_MCU
        if (bRefresh) {
            obdWriteCommand(pOBD, 0x12); // master activation
        }
    }
    if (pOBD->type == EPD579_792x272) {
        EPD_CMD2(pOBD, SSD1608_DISP_CTRL2, 0xf7);
        obdWriteCommand(pOBD, SSD1608_MASTER_ACTIVATE);
    }
    if (pOBD->type == EPD47_540x960) {
        uint16_t u16Args[7];
        EPD47_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0, NULL, 0, 0, pOBD->width, pOBD->height, 0);
        }
#endif
        EPD8951_CMD(pOBD, 0x0022); // LCD_IMG_END
        u16Args[0] = 0; // x
        u16Args[1] = 0; // y
        u16Args[2] = pOBD->native_width;
        u16Args[3] = pOBD->native_height;
        u16Args[4] = 0; // full update mode (2 seconds)
        u16Args[5] = 0x36e0; // _dev_memaddr_l
        u16Args[6] = 0x0012; // _dev_memaddr_h
        EPD8951_ARGS(pOBD, 0x0037, u16Args, 7); // IT8951_I80_CMD_DPY_BUF_AREA
    }
    if (pOBD->type == EPD293_128x296) {
        uint8_t ucTemp[4];
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
//        EPD293_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0x26, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPD_CMD2(pOBD, 0x4e, 0x00);
            ucTemp[0] = ucTemp[1] = 0;
            obdWriteCommand(pOBD, 0x4f);
            RawWriteData(pOBD, ucTemp, 2);
            EPDWriteImage(pOBD, 0x24, NULL, 0, 0, pOBD->width, pOBD->height, 0);
        }
#endif
        if (bRefresh) {
            EPD_CMD2(pOBD, 0x22, 0xc7);
            obdWriteCommand(pOBD, 0x20); // refresh
        }
    }
#ifdef FUTURE
    if (pOBD->type == EPD102_80x128) {
        EPD102_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0x10, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, 0x13, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send image to current and previous buffers
        }
#endif
        obdWriteCommand(pOBD, 0x12); // refresh
        _delay(10); // needed
    }
#endif // FUTURE
    if (pOBD->type == EPD154_152x152) {
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, UC8151_DTM1, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send image to current and previous buffers
        }
#endif
        if (bRefresh) {
            obdWriteCommand(pOBD, UC8151_PTOU);
            obdWriteCommand(pOBD, 0x12); // refresh
            _delay(10); // needed
        }
    }
  if (pOBD->type == EPD29_128x296) {
      EPDSendCMDSequence(pOBD, pOBD->pInitFull);
#ifndef WIMPY_MCU
      if (pOBD->ucScreen) {
          EPDWriteImage(pOBD, UC8151_DTM1, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send buffer to 'old' plane
          EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send buffer to 'new' plane
      }
#endif
      if (bRefresh) {
          EPD29_Finish(pOBD, false);
      }
  }
    if (pOBD->type == EPD27_176x264) {
        EPD27_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
        obdWriteCommand(pOBD, UC8151_DTM1);
        for (int i=0; i<5808; i++) { // all FF
            uint8_t uc = 0xff;
            RawWriteData(pOBD, &uc, 1);
        }
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0);
        }
#endif
        if (bRefresh) {
            obdWriteCommand(pOBD, UC8151_DRF);
        }
    }
    if (pOBD->type == EPD42_400x300) {
        //EPD42_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
      //  Serial.println("starting 4.2 update");
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
           EPDWriteImage(pOBD, UC8151_DTM1, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, UC8151_DTM2, NULL, 0, 0, pOBD->width, pOBD->height, 0);
        }
#endif
        if (bRefresh) {
            obdWriteCommand(pOBD, UC8151_DRF);
        }
    }
    if (pOBD->type == EPD29R_128x296) { // BLACK/WHITE/RED
        uint8_t ucTemp[4];
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, 0x26, NULL, 0, 0, pOBD->width, pOBD->height, 1); // red plane
            EPD_CMD2(pOBD, 0x4e, 0x00); // RAM X counter
            obdWriteCommand(pOBD, 0x4f);
            ucTemp[0] = (uint8_t)(pOBD->native_height-1);
            ucTemp[1] = (uint8_t)((pOBD->native_height-1)>>8);
            RawWriteData(pOBD, ucTemp, 2);
            EPDWriteImage(pOBD, 0x24, NULL, 0, 0, pOBD->width, pOBD->height, 0); // black/white plane
        }
#endif
        if (bRefresh) {
            EPD_CMD2(pOBD, 0x22, 0xc7); // update control 2
            obdWriteCommand(pOBD, 0x20); // master activation
        }
    }
    if (pOBD->type == EPD213_104x212 || pOBD->type == EPD213_122x250) {
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
        EPD213_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false);
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, SSD1608_WRITE_ALTRAM, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send image to current and previous buffers
        }
#endif
        if (bRefresh) {
            EPD154_Finish(pOBD, false);
        }
    } else if (pOBD->type == EPD154_200x200) {
        EPDSendCMDSequence(pOBD, pOBD->pInitFull);
        EPD154_Begin(pOBD, 0, 0, pOBD->width, pOBD->height, false); // x, y, w, h
#ifndef WIMPY_MCU
        if (pOBD->ucScreen) {
            EPDWriteImage(pOBD, SSD1608_WRITE_RAM, NULL, 0, 0, pOBD->width, pOBD->height, 0);
            EPDWriteImage(pOBD, SSD1608_WRITE_ALTRAM, NULL, 0, 0, pOBD->width, pOBD->height, 0); // send image to current and previous buffers
        }
#endif
        if (bRefresh) {
            EPD154_Finish(pOBD, false);
        }
    }
  EPDWaitBusy(pOBD, 0);
  EPDSleep(pOBD);
    return OBD_SUCCESS;
} /* EPDDumpBuffer() */
//
// Special case for ST7302
//
#ifndef WIMPY_MCU
void obdST7302SetPos(OBDISP *pOBD, int x, int y)
{
uint8_t ucTemp[4];
    
    obdWriteCommand(pOBD, 0x2a); // Column set
    ucTemp[0] = 0x19 + (y/12); // start x (we treat it as y)
    ucTemp[1] = 0x27; // end x
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0x2b); // Row set
    ucTemp[0] = (x/2); // start y (we treat it as x)
    ucTemp[1] = 0x80; // end y
    RawWriteData(pOBD, ucTemp, 2);
    obdWriteCommand(pOBD, 0x2c); // memory write
} /* obdST7302SetPos() */
//
// Special case for ST7302
//
static int ST7302DumpBuffer(OBDISP *pOBD, uint8_t *pBuffer)
{
uint8_t ucPixels[40];
int x, y, iPitch, count;
uint8_t ucMask, uc1, *s, *d;
    
    iPitch = pOBD->width;
// Shift out the image in pairs of lines
    ucPixels[0] = 0x40;
    switch (pOBD->iOrientation) {
        case 0:
        case 180:
            for (x = 0; x < pOBD->width; x += 2) { // a pair of columns at a time
                d = ucPixels+1;
                ucMask = 1;
                uc1 = 0;
                count = 0;
                if (pOBD->iOrientation == 180) {
                    ucMask = 1<<((pOBD->height-1) & 7);
                    s = &pBuffer[pOBD->width-2-x + ((pOBD->height-1)>>3)*iPitch];
                    for (y = pOBD->height-1; y >= 0; y--) {
                        uc1 <<= 2;
                        if (s[0] & ucMask) uc1 |= 1;
                        if (s[1] & ucMask) uc1 |= 2;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                            uc1 = 0;
                        }
                        ucMask >>= 1;
                        if (ucMask == 0) {
                            ucMask = 0x80;
                            s -= iPitch;
                        } // next line
                    } // for y
                    if (count) {
                        uc1 <<= (count*2);
                        *d++ = uc1;
                    }
                } else {
                    s = &pBuffer[x];
                    uc1 = 0;
                    for (y=0; y < pOBD->height+3; y++) {
                        uc1 <<= 2;
                        if (s[0] & ucMask) uc1 |= 2;
                        if (s[1] & ucMask) uc1 |= 1;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                            uc1 = 0;
                        }
                        ucMask <<= 1;
                        if (ucMask == 0) {
                            ucMask = 1;
                            s += iPitch;
                        } // next line
                    } // for y
                } // flipped
                obdSetPosition(pOBD, (pOBD->flip) ? 248-x : x, 0, 1);
                RawWrite(pOBD, ucPixels, 3 + (int)(d - ucPixels));
            } // for x
            break;
        case 90:
        case 270: // x/y swapped
            if (pOBD->iOrientation == 270) {
                for (y=pOBD->height-2; y >= 0; y-=2) {
                    ucMask = 1<<(y & 7);
                    s = &pBuffer[(y>>3)*pOBD->width];
                    count = 0;
                    d = ucPixels+1;
                    uc1 = 0;
                    for (x=0; x<pOBD->width; x++) {
                        uc1 <<= 2;
                        if (s[x] & ucMask) uc1 |= 1;
                        if (s[x] & (ucMask << 1)) uc1 |= 2;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                        }
                    } // for x
                    obdSetPosition(pOBD, pOBD->height - 2 - y, 0, 1);
                    RawWrite(pOBD, ucPixels, 3 + (int)(d - ucPixels));
                } // for y
            } else { // 90
                for (y=0; y < pOBD->height; y+=2) {
                    ucMask = 1<<(y & 7);
                    s = &pBuffer[(y>>3)*pOBD->width];
                    count = 0;
                    d = ucPixels+1;
                    uc1 = 0;
                    for (x=pOBD->width-1; x>=0; x--) {
                        uc1 <<= 2;
                        if (s[x] & ucMask) uc1 |= 2;
                        if (s[x] & (ucMask << 1)) uc1 |= 1;
                        count++;
                        if (count == 4) { // finish the byte
                            *d++ = uc1;
                            count = 0;
                        }
                    } // for x
                    obdSetPosition(pOBD, y, 0, 1);
                    RawWrite(pOBD, ucPixels, 3 + (int)(d - ucPixels));
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
uint8_t c, ucInvert, *s, *d, ucStart;
uint8_t ucLineBuf[56];
int iPitch = pOBD->native_width / 8;
static uint8_t ucVCOM = 0;
int iBit;
uint8_t ucMask;

  ucInvert = (pOBD->invert) ? 0x00 : 0xff;
  digitalWrite(pOBD->iCSPin, HIGH); // active high
 
    ucLineBuf[0] = 0;
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
        } // for y
        // write this line to the display
        ucLineBuf[iPitch+2] = 0; // end of line
        RawWrite(pOBD, ucLineBuf, iPitch+3);
     } // for x
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
// Dump a portion of the display buffer
// optional buffer pointer if there is no back buffer defined
//
int obdDumpPartial(OBDISP *pOBD, int startx, int starty, int width, int height, uint8_t *pBuffer)
{
    uint8_t *s = pBuffer;
    int iPitch;
    uint8_t ucCMD1, ucCMD2;
    
//    Serial.println("entering partial update");
    if (pOBD == NULL) return OBD_ERROR_BAD_PARAMETER;
    if (startx < 0 || starty < 0 || startx+width > pOBD->width || starty+height > pOBD->height)
        return OBD_ERROR_BAD_PARAMETER;
    if (s == NULL) {
        s = pOBD->ucScreen;
        iPitch = pOBD->width;
        s += startx + ((starty>>3) * iPitch);
    } else {
        iPitch = width;
    }
    EPDWakeUp(pOBD);
    EPDSendCMDSequence(pOBD, pOBD->pInitFull);
    EPDSetPosition(pOBD, startx, starty, width, height);
    if (pOBD->chip_type == OBD_CHIP_UC8151) {
        ucCMD1 = UC8151_DTM1;
        ucCMD2 = UC8151_DTM2;
    } else {
        ucCMD1 = SSD1608_WRITE_RAM;
        ucCMD2 = SSD1608_WRITE_ALTRAM;
    }
#ifndef WIMPY_MCU
    if (s) {
        if (s != pOBD->ucScreen) {
            startx = starty = 0; // start from 0 if passing your own buffer
        }
        EPDWriteImage(pOBD, ucCMD2, s, startx, starty, width, height, 0);
    }
#endif
    // tell EPD to do a partial update of the requested area
    if (pOBD->chip_type == OBD_CHIP_UC8151) {
      //  Serial.println("About to send refresh cmd");
        obdWriteCommand(pOBD, UC8151_DSP); // stop data
        obdWriteCommand(pOBD, UC8151_DRF); // refresh
        _delay(10); // needed
        EPDWaitBusy(pOBD, 0);
        obdWriteCommand(pOBD, UC8151_PTOU); // disable partial mode
    } else if (pOBD->chip_type == OBD_CHIP_SSD16xx) {
        EPD_CMD2(pOBD, SSD1608_DISP_CTRL2, 0xf4);
        obdWriteCommand(pOBD, SSD1608_MASTER_ACTIVATE);
        EPDWaitBusy(pOBD, false);
        obdWriteCommand(pOBD, SSD1608_NOP);
    }
    return OBD_SUCCESS;
} /* obdDumpPartial() */
//
// Dump a screen faster to an e-ink display
//
int obdDumpFast(OBDISP *pOBD, int startx, int starty, int width, int height)
{
    if (pOBD->type >= EPD42_400x300) { // it's an e-ink panel
        if (pOBD->iFlags & OBD_HAS_FAST_UPDATE) {
            int rc = EPDDumpFast(pOBD, NULL, startx, starty, width, height);
            pOBD->iFlags &= ~OBD_FULLUPDATE; // mark it for later
            return rc;
        } else {
            return EPDDumpBuffer(pOBD, true);
        }
    } else { // use the default full screen dump function
        return obdDumpBuffer(pOBD, NULL);
    }
} /* obdDumpFast() */
//
// Dump a screen's worth of data directly to the display
// Try to speed it up by comparing the new bytes with the existing buffer
//
int obdDumpBuffer_2(OBDISP *pOBD, uint8_t *pBuffer, int bRefresh)
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
    
  if (pOBD->type >= EPD42_400x300) // all e-ink panels
  {
     return EPDDumpBuffer(pOBD, bRefresh);
  }
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
  if (pOBD->type == LCD_ST7302) // special case for ST7302
  {
      return ST7302DumpBuffer(pOBD, pBuffer);
  }
#endif // !WIMPY_MCU
  if (pOBD->type >= SHARP_144x168) // special case for Sharp Memory LCDs
  {
    return SharpDumpBuffer(pOBD, pBuffer);
  }
  iLines = (pOBD->native_height+7) >> 3;
    // 0/180 we can send the 8 lines of pixels straight through
    if (pOBD->iOrientation == 0 || pOBD->iOrientation == 180) {
        if (pOBD->iOrientation == 0 || (pOBD->iOrientation == 180 && pOBD->can_flip)) {
            for (y=0; y<iLines; y++) {
                obdSetPosition(pOBD, 0, y*8, 1);
                obdWriteDataBlock(pOBD, pBuffer, pOBD->width, 1);
                pBuffer += pOBD->width;
            }
        } else { // have to manually do 180 flipped
            for (y=iLines-1; y>=0; y--) {
                obdSetPosition(pOBD, 0, y*8, 1);
                for (int x=0; x<pOBD->width; x++) {
                    u8Cache[pOBD->width-1-x] = ucMirror[pBuffer[x]];
                }
                obdWriteDataBlock(pOBD, u8Cache, pOBD->width, 1);
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
} /* obdDumpBuffer_2() */

int obdDumpBuffer(OBDISP *pOBD, uint8_t *pBuffer)
{
    return obdDumpBuffer_2(pOBD, pBuffer, true);
}

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
// Experimental bufferless E-ink drawing code
//
void EPDSetPosition(OBDISP *pOBD, int x, int y, int cx, int cy)
{
uint8_t uc[12];
int i, tx, ty;
    
    // We need to rotate the drawing direction since native direction for EPDs
    // is horizontal bytes across the X direction with the MSB on the left
    if (pOBD->iOrientation == 90 || pOBD->iOrientation == 270) {
        if (x + cx > pOBD->native_height)
            cx = pOBD->native_height - x;
        if (y + cy > pOBD->native_width)
            cy = pOBD->native_width - y;
        y &= 0xff8; // fix to byte boundary
        if ((pOBD->iFlags & OBD_FLIPV) || (pOBD->ucScreen == NULL/* && pOBD->chip_type == OBD_CHIP_SSD16xx*/)) // flipped Y when drawing with no back buffer
            tx = (pOBD->native_width /*+ 7*/ - y - cy)/8;
        else
            tx = y/8;
        ty = x;
        // swap width/height
        i = cx;
        cx = cy;
        cy = i;
    } else {
        tx = (x+7)/8;
        ty = y;
        if (ty+cy > pOBD->native_height) {
            cy = pOBD->native_height - ty;
        }
        if ((tx*8)+cx > pOBD->native_width) {
            cx = pOBD->native_width - (tx*8);
        }
    }
    switch (pOBD->chip_type) {
        case OBD_CHIP_SSD16xx:
            if (pOBD->type == EPD579_792x272) {  // special case
                EPD_CMD2(pOBD, SSD1608_DATA_MODE, 0x02);
                if (cx > 400 && cy == 8) { // a single line of graphics
                    cx = 8;
                    cy = 271-ty; // different mem direction
                }
                if (cx > 400) cx = 400; // two sets of 400 pixels
                obdWriteCommand(pOBD, SSD1608_SET_RAMXPOS);
                uc[1] = tx; // start x (byte boundary)
                uc[0] = tx+(cx>>3); // end x
                RawWriteData(pOBD, uc, 2);
                obdWriteCommand(pOBD, SSD1608_SET_RAMYPOS);
                uc[0] = (uint8_t)ty; // start y
                uc[1] = (uint8_t)(ty>>8);
                uc[2] = (uint8_t)(ty+cy-1); // end y
                uc[3] = (uint8_t)((ty+cy-1)>>8);
                RawWriteData(pOBD, uc, 4);
                EPD_CMD2(pOBD, SSD1608_SET_RAMXCOUNT, 0xf);
                obdWriteCommand(pOBD, SSD1608_SET_RAMYCOUNT);
                uc[0] = uc[1] = 0;
                RawWriteData(pOBD, uc, 2);
                if (pOBD->iFG == OBD_RED) // write to red plane
                    obdWriteCommand(pOBD, SSD1608_WRITE_ALTRAM);
                else
                    obdWriteCommand(pOBD, SSD1608_WRITE_RAM); // prepare to start writing pixels
            } else {
                obdWriteCommand(pOBD, SSD1608_SET_RAMXPOS);
                uc[0] = tx + pOBD->x_offset; // start x (byte boundary)
               // if (cx == pOBD->native_width && (pOBD->native_width & 7) != 0)
               //     cx += 7; // needs one more byte for the full display width
                uc[1] = tx+((cx-1)>>3) + pOBD->x_offset; // end x
                RawWriteData(pOBD, uc, 2);
                obdWriteCommand(pOBD, SSD1608_SET_RAMYPOS);
                if (pOBD->iFlags & OBD_FLIPV) {
                    uc[0] = (uint8_t)(ty+cy-1); // start y
                    uc[1] = (uint8_t)((ty+cy-1)>>8);
                    uc[2] = (uint8_t)ty; // end y
                    uc[3] = (uint8_t)(ty>>8);
                } else {
                    uc[0] = (uint8_t)ty; // start y
                    uc[1] = (uint8_t)(ty>>8);
                    uc[2] = (uint8_t)(ty+cy-1); // end y
                    uc[3] = (uint8_t)((ty+cy-1)>>8);
                }
                RawWriteData(pOBD, uc, 4);
                // set ram counter to start of this region
                EPD_CMD2(pOBD, SSD1608_SET_RAMXCOUNT, tx);
                uc[0] = ty;
                uc[1] = (ty>>8);
                obdWriteCommand(pOBD, SSD1608_SET_RAMYCOUNT);
                RawWriteData(pOBD, uc, 2);
                if (pOBD->iFlags & OBD_FLIPV)
                    EPD_CMD2(pOBD, SSD1608_DATA_MODE, 0x1);
                else
                    EPD_CMD2(pOBD, SSD1608_DATA_MODE, 0x3);
                if (pOBD->iFG == OBD_RED) // write to red plane
                    obdWriteCommand(pOBD, SSD1608_WRITE_ALTRAM);
                else
                    obdWriteCommand(pOBD, SSD1608_WRITE_RAM); // prepare to start writing pixels
            }
            break;
        case OBD_CHIP_UC8151:
            obdWriteCommand(pOBD, UC8151_PTIN); // partial in
            obdWriteCommand(pOBD, UC8151_PTL); // partial window
            i = 0;
            tx *= 8;
            if (pOBD->native_width > 256) { // need 2 bytes per x
                uc[i++] = (uint8_t)(tx>>8); // start x
                uc[i++] = (uint8_t)tx;
                uc[i++] = (uint8_t)((tx+cx-1)>>8); // end x
                uc[i++] = (uint8_t)((tx+cx-1) | 7);
            } else {
                uc[i++] = tx; // start x
                uc[i++] = (tx+cx-1) | 7; // end x
            }
            if (pOBD->iFlags & OBD_FLIPV) {
                uc[i++] = (uint8_t)((ty+cy-1)>>8); // start y
                uc[i++] = (uint8_t)(ty+cy-1);
                uc[i++] = (uint8_t)(ty>>8); // end y
                uc[i++] = (uint8_t)ty;
            } else {
                uc[i++] = (uint8_t)(ty>>8); // start y
                uc[i++] = (uint8_t)ty;
                uc[i++] = (uint8_t)((ty+cy-1)>>8); // end y
                uc[i++] = (uint8_t)(ty+cy-1);
            }
            uc[i++] = 0; // stay inside the partial window3
            RawWriteData(pOBD, uc, i);
            if (pOBD->iFlags & OBD_3COLOR) {
                if (pOBD->iFG == OBD_RED) // write to red plane
                    obdWriteCommand(pOBD, UC8151_DTM2);
                else
                    obdWriteCommand(pOBD, UC8151_DTM1); // prepare to start writing pixels
            } else {
                obdWriteCommand(pOBD, UC8151_DTM2);
            }
            break;
        default: // error
            break;
    }
} /* EPDSetPosition() */
//
// Send commands to position the "cursor" (aka memory write address)
// to the given row and column
//
void obdSetPosition(OBDISP *pOBD, int x, int y, int bRender)
{
unsigned char buf[4];
int iPitch = pOBD->width;

    // e-paper direct access
    if (pOBD->type >= EPD42_400x300 && !pOBD->ucScreen) { // immediate mode for e-paper displays
        EPDSetPosition(pOBD, x, y, 800, 8); // let the width get corrected in the command prep
        return;
    }
#if !defined( WIMPY_MCU ) && !defined(__AVR__)
    if (pOBD->type == LCD_ST7302) { // special case for ST7302
        obdST7302SetPos(pOBD, x, y);
    }
#endif // !WIMPY_MCU
    y >>= 3; // DEBUG - since we address the display by lines of 8 pixels
  pOBD->iScreenOffset = (y*iPitch)+x;

  if (pOBD->type == LCD_VIRTUAL || pOBD->type >= SHARP_144x168)
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
int iRedOffset = 0;
    
  iPitch = pOBD->width;
  iBufferSize = iPitch * ((pOBD->height+7) / 8);
    if (pOBD->iFG == OBD_RED) {
        iRedOffset = pOBD->width * ((pOBD->height+7)/8);
    }
// Keep a copy in local buffer
if (pOBD->ucScreen && (iLen + pOBD->iScreenOffset) <= iBufferSize)
{
  memcpy(&pOBD->ucScreen[iRedOffset + pOBD->iScreenOffset], ucBuf, iLen);
  pOBD->iScreenOffset += iLen;
  // wrap around ?
  if (pOBD->iScreenOffset >= iBufferSize)
    pOBD->iScreenOffset -= iBufferSize;
}
if (pOBD->type == LCD_VIRTUAL || pOBD->type == SHARP_144x168 || pOBD->type == SHARP_400x240)
  return; // nothing else to do
#ifndef MEMORY_ONLY
  if (bRender)
  {
      RawWriteData(pOBD, ucBuf, iLen);
  }
#endif // MEMORY_ONLY
} /* obdWriteDataBlock() */
