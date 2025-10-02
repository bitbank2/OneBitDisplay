#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#ifndef CONSUMER
#define CONSUMER "Consumer"
#endif
#include <gpiod.h>
#define pgm_read_byte(p)  *(uint8_t *)(p)
#define pgm_read_word(p)  *(uint16_t *)(p)
#define memcpy_P memcpy
#define MAX_CACHE 1024
static uint8_t u8Temp[40]; // for stretched character drawing
static volatile uint8_t u8End = 0;
static uint8_t u8Cache[MAX_CACHE];
struct gpiod_chip *chip = NULL;
struct gpiod_line *lines[128];
//
// I/O wrapper functions for Linux
//
int digitalRead(int iPin)
{
  return gpiod_line_get_value(lines[iPin]);
} /* digitalRead() */

void digitalWrite(int iPin, int iState)
{
   gpiod_line_set_value(lines[iPin], iState);
} /* digitalWrite() */

void _delay(int l)
{
    usleep(l * 1000);
}

void pinMode(int iPin, int iMode)
{
   if (chip == NULL) {
       chip = gpiod_chip_open_by_name("gpiochip0");
   }
   lines[iPin] = gpiod_chip_get_line(chip, iPin);
   if (iMode == OUTPUT) {
       gpiod_line_request_output(lines[iPin], CONSUMER, 0);
   } else if (iMode == INPUT_PULLUP) {
       gpiod_line_request_input_flags(lines[iPin], CONSUMER, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
   } else { // plain input
       gpiod_line_request_input(lines[iPin], CONSUMER);
   }
} /* pinMode() */

void delay(uint32_t u32)
{
        usleep(u32*1000);
} /* delay() */

void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS)
{
} /* initSPI() */

// Initialize the I2C bus on Linux
void I2CInit(BBI2C *pI2C, uint32_t iClock)
{
char filename[32];
int iChannel = pI2C->iSDA;

        sprintf(filename, "/dev/i2c-%d", iChannel);
        if ((pI2C->file_i2c = open(filename, O_RDWR)) < 0)
        {
                fprintf(stderr, "Failed to open the i2c bus\n");
                return;
        }
} /* I2CInit() */
//
// Test if an I2C address responds
// returns 1 for success, 0 for failure
//
uint8_t I2CTest(BBI2C *pI2C, uint8_t addr)
{
uint8_t response = 0;
    if (ioctl(pI2C->file_i2c, I2C_SLAVE, addr) >= 0) {
            // probe this address
        uint8_t ucTemp;
        if (read(pI2C->file_i2c, &ucTemp, 1) >= 0)
            response = 1;
    }
    return response;
} /* I2CTest() */
//
// Read n bytes from the given I2C address
//
int I2CRead(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = read(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CRead() */
//
// Read n bytes from the given address, after setting the register number
//
int I2CReadRegister(BBI2C *pI2C, uint8_t iAddr, uint8_t u8Register, uint8_t *pData, int iLen)
{
int rc;
        // Reading from an I2C device involves first writing the 8-bit register
        // followed by reading the data
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = write(pI2C->file_i2c, &u8Register, 1); // write the register value
        if (rc == 1)
        {
                rc = read(pI2C->file_i2c, pData, iLen);
        }
        return rc;

} /* I2CReadRegister() */
//
// Write n bytes to the given address
//
int I2CWrite(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = write(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CWrite() */

// Write raw (unfiltered) bytes directly to I2C or SPI
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
  if (pOBD->com_mode == COM_I2C) {// I2C device
      I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, pData, iLen);
  } else { // must be SPI
      digitalWrite(pOBD->iDCPin, (pData[0] != 0));
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, LOW);
         // DEBUG - todo
      //WriteSPI(pOBD->bbi2c.file_i2c, &pData[1], iLen-1);
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, HIGH);
      //obdSetDCMode(pOBD, MODE_DATA);
  }
} /* RawWrite() */

void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen)
{
uint8_t u8Temp[256];
  if (pOBD->com_mode == COM_I2C) {// I2C device
      u8Temp[0] = 0x40; // data introducer
      memcpy(&u8Temp[1], pData, iLen);
      I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, u8Temp, iLen+1);
  } else { // must be SPI
      digitalWrite(pOBD->iDCPin, 1);
//    if (pOBD->iFlags & OBD_CS_EVERY_BYTE) {
//       for (int i=0; i<iLen; i++) {
//          digitalWrite(pOBD->iCSPin, LOW);
          // DEBUG - todo
        //WriteSPI(pOBD->bbi2c.file_i2c, &pData[i], 1);
//          digitalWrite(pOBD->iCSPin, HIGH);
//       }
//    } else {
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, LOW);
      //WriteSPI(pOBD->bbi2c.file_i2c, pData, iLen);
      if (pOBD->iCSPin != 0xff &&pOBD->chip_type != OBD_CHIP_SHARP)
         digitalWrite(pOBD->iCSPin, HIGH);
//    }
     //obdSetDCMode(pOBD, MODE_DATA);
  }
} /* RawWriteData() */

