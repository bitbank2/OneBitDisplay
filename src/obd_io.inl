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
// I/O functions for the OneBitDisplay library
//
// This define (WIMPY_MCU) precludes the use of hardware interfaces
// such as I2C & SPI
//
#if defined (__AVR_ATtiny85__) || defined (ARDUINO_ARCH_MCS51)
#define WIMPY_MCU
#endif

#define MAX_CACHE 192
static uint8_t u8Cache[MAX_CACHE]; // for faster character drawing
static volatile uint8_t u8End = 0;
static uint8_t u8Temp[40]; // for stretched character drawing

#ifdef _LINUX_
#define PROGMEM
#define false 0
#define true 1
#define LOW 0
#define HIGH 1
#ifndef I2C_SLAVE
#define I2C_SLAVE 0
#define INPUT GPIO_IN
#define OUTPUT GPIO_OUT
#define INPUT_PULLUP GPIO_IN_PULLUP
#define HIGH 1
#define LOW 0
#endif
#if defined(_LINUX_) || defined(ARDUINO_ARCH_MCS51)
#define memcpy_P memcpy
#endif

void obdSetDCMode(OBDISP *pOBD, int iMode);
static uint8_t pgm_read_byte(const uint8_t *ptr);
static void digitalWrite(int iPin, int iState) {
#ifndef MEMORY_ONLY
   AIOWriteGPIO(iPin, iState);
#endif
}
static void pinMode(int iPin, int iMode)
{
#ifndef MEMORY_ONLY
   AIOAddGPIO(iPin, iMode);
#endif
} /* pinMode() */
static void delayMicroseconds(int iTime)
{
	usleep(iTime);
} /* delayMicroseconds() */
static int digitalRead(int iPin)
{
  return AIOReadGPIO(iPin);
} /* digitalRead() */
#endif // _LINUX_

#if !defined( _LINUX_ ) && !defined( WIMPY_MCU )
#include <SPI.h>

#ifdef ARDUINO_ARCH_RP2040
MbedSPI *mySPI;
#else
SPIClass *mySPI = &SPI;
#endif
#endif // !_LINUX_

// Initialize SPI
void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS)
{
#ifdef WIMPY_MCU
    (void)iSpeed; (void)iMOSI; (void)iCLK; (void)iCS;
#endif
    
if (pOBD->bBitBang)
    return;
#ifdef _LINUX_
    pOBD->bbi2c.file_i2c = AIOOpenSPI(SPI_BUS_NUMBER, iSpeed);
#elif defined ( ARDUINO_ARCH_RP2040 )
    if (iMOSI == -1 || iMOSI == 0xff) {
        iMOSI = MOSI; iCLK = SCK; // use the default pins
    }
    mySPI = new MbedSPI(-1,iMOSI,iCLK);
    mySPI->begin();
#elif defined( ARDUINO_ARCH_ESP32 ) || defined( RISCV )
    if (iMOSI != -1 && iMOSI != 0xff) {
        mySPI->begin(iCLK, -1, iMOSI, iCS);
    } else {
        mySPI->begin();
    }
#elif !defined (WIMPY_MCU) // simple (default pin) SPI
    (void)iMOSI; (void)iCLK; (void)iCS;
    mySPI->begin();
    mySPI->beginTransaction(SPISettings(iSpeed, MSBFIRST, SPI_MODE0));
#ifndef ARDUINO_ARCH_NRF52
    mySPI->endTransaction(); // N.B. - if you call beginTransaction() again without a matching endTransaction(), it will hang on ESP32
    // BUT!!! if you do call endTransaction on NRF52, it won't send any data at all
#endif
#endif // _LINUX_
} /* initSPI() */

//
// Bit Bang the data on GPIO pins
//
void SPI_BitBang(OBDISP *pOBD, uint8_t *pData, int iLen, uint8_t iMOSIPin, uint8_t iSCKPin)
{
int i;
uint8_t c;

// We can access the GPIO ports much quicker on AVR by directly manipulating
// the port registers
#ifdef __AVR__
volatile uint8_t *outSCK, *outMOSI; // port registers for fast I/O
uint8_t port, bitSCK, bitMOSI; // bit mask for the chosen pins

    port = digitalPinToPort(iMOSIPin);
    outMOSI = portOutputRegister(port);
    bitMOSI = digitalPinToBitMask(iMOSIPin);
    port = digitalPinToPort(iSCKPin);
    outSCK = portOutputRegister(port);
    bitSCK = digitalPinToBitMask(iSCKPin);

#endif

   while (iLen)
   {
      c = *pData++;
      iLen--;
      if (pOBD->iDCPin == 0xff && pOBD->chip_type != OBD_CHIP_SHARP) // 3-wire SPI, write D/C bit first
      {
#ifdef __AVR__
          if (pOBD->mode == MODE_DATA)
             *outMOSI |= bitMOSI;
          else
             *outMOSI &= ~bitMOSI;
          *outSCK |= bitSCK; // toggle clock
          *outSCK &= ~bitSCK; // no delay needed on SPI devices since AVR is slow
#else
          digitalWrite(iMOSIPin, (pOBD->mode == MODE_DATA));
          digitalWrite(iSCKPin, HIGH);
          delayMicroseconds(1);
      digitalWrite(iSCKPin, LOW);
#endif
      }
#ifdef FUTURE
      if (c == 0 || c == 0xff) // quicker for all bits equal
      {
#ifdef __AVR__
         if (c & 1)
            *outMOSI |= bitMOSI;
         else
            *outMOSI &= ~bitMOSI;
         for (i=0; i<8; i++)
         {
            *outSCK |= bitSCK;
            *outSCK &= ~bitSCK;
         }
#else
         digitalWrite(iMOSIPin, (c & 1));
         for (i=0; i<8; i++)
         {
            digitalWrite(iSCKPin, HIGH);
            delayMicroseconds(1);
            digitalWrite(iSCKPin, LOW);
         }
#endif
      }
      else
#endif // FUTURE
      {
         for (i=0; i<8; i++)
         {
#ifdef __AVR__
            if (c & 0x80) // MSB first
               *outMOSI |= bitMOSI;
            else
               *outMOSI &= ~bitMOSI;
            *outSCK |= bitSCK;
            c <<= 1;
            *outSCK &= ~bitSCK;
#else
            digitalWrite(iMOSIPin,  (c & 0x80) != 0); // MSB first
            delayMicroseconds(1);
            digitalWrite(iSCKPin, HIGH);
            c <<= 1;
            delayMicroseconds(1);
            digitalWrite(iSCKPin, LOW);
            delayMicroseconds(1);
#endif
        }
      }
   }
} /* SPI_BitBang() */

// wrapper/adapter functions to make the code work on Linux
#if defined( _LINUX_ ) || defined(ARDUINO_ARCH_MCS51)
static uint8_t pgm_read_byte(const uint8_t *ptr)
{
  return *ptr;
}
static int16_t pgm_read_word(const uint8_t *ptr)
{
  return ptr[0] + (ptr[1]<<8);
}
#if !defined( MEMORY_ONLY )  && !defined( WIMPY_MCU )
int I2CReadRegister(BBI2C *pI2C, uint8_t addr, uint8_t reg, uint8_t *pBuf, int iLen)
{
int rc;
  rc = write(pI2C->file_i2c, &reg, 1);
  rc = read(pI2C->file_i2c, pBuf, iLen);
  return (rc > 0);
}
int I2CRead(BBI2C *pI2C, uint8_t addr, uint8_t *pBuf, int iLen)
{
int rc;
  rc = read(pI2C->file_i2c, pBuf, iLen);
  return (rc > 0);
}
void I2CInit(BBI2C *pI2C, uint32_t iSpeed)
{
char filename[32];

  sprintf(filename, "/dev/i2c-%d", pI2C->iSDA); // I2C bus number passed in SDA pin
  if ((pI2C->file_i2c = open(filename, O_RDWR)) < 0)
      return;// 1;
  if (ioctl(pI2C->file_i2c, I2C_SLAVE, pI2C->iSCL) < 0) // set slave address
  {
     close(pI2C->file_i2c);
     pI2C->file_i2c = 0;
      return; // 1;
  }
    return; // 0;
}
#endif // MEMORY_ONLY

// Write raw (unfiltered) bytes directly to I2C or SPI
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
#if !defined( MEMORY_ONLY ) && !defined(WIMPY_MCU)
  if (pOBD->com_mode == COM_I2C) {// I2C device
      write(pOBD->bbi2c.file_i2c, pData, iLen);
  } else { // must be SPI
      obdSetDCMode(pOBD, (pData[0] == 0) ? MODE_COMMAND : MODE_DATA);
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, LOW);
      AIOWriteSPI(pOBD->bbi2c.file_i2c, &pData[1], iLen-1);
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, HIGH);
      //obdSetDCMode(pOBD, MODE_DATA);
  }
#else
    (void)pOBD; (void)pData; (void)iLen;
#endif // MEMORY_ONLY
} /* RawWrite() */
void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen)
{
#if !defined( MEMORY_ONLY ) && !defined(WIMPY_MCU)
  if (pOBD->com_mode == COM_I2C) {// I2C device
      write(pOBD->bbi2c.file_i2c, pData, iLen);
  } else { // must be SPI
      obdSetDCMode(pOBD, MODE_DATA);
    if (pOBD->iFlags & OBD_CS_EVERY_BYTE) {
       for (int i=0; i<iLen; i++) {
          digitalWrite(pOBD->iCSPin, LOW);
      AIOWriteSPI(pOBD->bbi2c.file_i2c, &pData[i], 1);
      digitalWrite(pOBD->iCSPin, HIGH);
       }
    } else {
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, LOW);
      AIOWriteSPI(pOBD->bbi2c.file_i2c, pData, iLen);
      if (pOBD->iCSPin != 0xff &&pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, HIGH);
    }
      //obdSetDCMode(pOBD, MODE_DATA);
  }
#else
    (void)pOBD; (void)pData; (void)iLen;
#endif // MEMORY_ONLY
} /* RawWriteData() */
#else // Arduino
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
#if !defined( WIMPY_MCU )
  if (pOBD->com_mode == COM_SPI) // we're writing to SPI, treat it differently
  {
    if (pOBD->iDCPin != 0xff)
    {
      digitalWrite(pOBD->iDCPin, (pData[0] == 0) ? LOW : HIGH); // data versus command
    }
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
    {
        digitalWrite(pOBD->iCSPin, LOW);
    }
#ifdef HAL_ESP32_HAL_H_
   {
   uint8_t ucTemp[1024];
       if (pOBD->bBitBang)
           SPI_BitBang(pOBD, &pData[1], iLen-1, pOBD->iMOSIPin, pOBD->iCLKPin);
       else
           mySPI->transferBytes(&pData[1], ucTemp, iLen-1);
   }
#else
    for (int i=1; i<iLen; i++) {
        if (pOBD->bBitBang)
            SPI_BitBang(pOBD, &pData[i], 1, pOBD->iMOSIPin, pOBD->iCLKPin);
        else
            mySPI->transfer(pData[i]);
    }
#endif
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP) {
          digitalWrite(pOBD->iCSPin, HIGH);
      }
      if (pOBD->iDCPin != 0xff && pData[0] == 0) { // was command mode, set back to data mode
          digitalWrite(pOBD->iDCPin, HIGH);
      }
  }
  else // must be I2C
#endif // !WIMPY_MCU
  {
    if (pOBD->bbi2c.bWire && iLen > 32) // Hardware I2C has write length limits
    {
       iLen--; // don't count the 0x40 byte the first time through
       while (iLen >= 31) // max 31 data byes + data introducer
       {
          I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, pData, 32);
          iLen -= 31;
          pData += 31;
          pData[0] = 0x40;
       }
       if (iLen) iLen++; // If remaining data, add the last 0x40 byte
    }
    if (iLen) // if any data remaining
    {
      I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, pData, iLen);
    }
  } // I2C
} /* RawWrite() */
void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen)
{
#if !defined( WIMPY_MCU )
  if (pOBD->com_mode == COM_SPI) // we're writing to SPI, treat it differently
  {
    if (pOBD->iDCPin != 0xff)
      digitalWrite(pOBD->iDCPin, HIGH); // data mode
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
      digitalWrite(pOBD->iCSPin, LOW);
//#ifdef HAL_ESP32_HAL_H_
//   {
//   uint8_t ucTemp[1024];
//        mySPI->transferBytes(pData, ucTemp, iLen);
//   }
//#else
    if (pOBD->iFlags & OBD_CS_EVERY_BYTE) {
        for (int i=0; i<iLen; i++) {
            digitalWrite(pOBD->iCSPin, LOW);
            mySPI->transfer(pData[i]);
            digitalWrite(pOBD->iCSPin, HIGH);
        }
      } else { // no need for CS on every byte
          for (int i=0; i<iLen; i++) {
              mySPI->transfer(pData[i]);
          }
      }
//#endif
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
      digitalWrite(pOBD->iCSPin, HIGH);
  }
  else // must be I2C
#endif // !WIMPY_MCU
{
    u8Temp[0] = 0x40; // data prefix byte
    while (iLen >= 31) { // max 31 data byes + data introducer
        memcpy(&u8Temp[1], pData, 31);
        I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, u8Temp, 32);
        iLen -= 31;
        pData += 31;
    } // while >= 31 bytes to send
    if (iLen) // if any data remaining
    {
        memcpy(&u8Temp[1], pData, iLen);
        I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, u8Temp, iLen+1);
    }
  } // I2C
} /* RawWriteData() */
#endif // _LINUX_

#ifdef _LINUX_
void _delay(int iDelay)
{
    usleep(iDelay * 1000);
} /* _delay() */
#else // Arduino
void _delay(int iDelay)
{
#ifdef NOT_HAL_ESP32_HAL_H_
// light sleep to save power
    esp_sleep_enable_timer_wakeup(iDelay * 1000);
//    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
#else // any other platform
    delay(iDelay); // use the Arduino delay function
#endif // ESP32
}
#endif // _LINUX_
