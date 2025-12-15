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
static struct gpiod_chip *chip = NULL;
#ifdef GPIOD_API
static struct gpiod_line *lines[128];
#else
static struct gpiod_line_request *lines[128];
#endif
//
// I/O wrapper functions for Linux
//
static void digitalWrite(int iPin, int iState)
{
    if (lines[iPin] == 0) return;
#ifdef GPIOD_API // old 1.6 API
    gpiod_line_set_value(lines[iPin], iState);
#else // new 2.x API
    gpiod_line_request_set_value(lines[iPin], iPin, (iState) ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
#endif
} /* digitalWrite() */

static void _delay(int l)
{
    usleep(l * 1000);
}

static void pinMode(int iPin, int iMode)
{
#ifdef GPIOD_API // old 1.6 API
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
#else // new 2.x API
   struct gpiod_line_settings *settings;
   struct gpiod_line_config *line_cfg;
   struct gpiod_request_config *req_cfg;
   chip = gpiod_chip_open("/dev/gpiochip0");
   if (!chip) {
        printf("chip open failed\n");
           return;
   }
   settings = gpiod_line_settings_new();
   if (!settings) {
        printf("line_settings_new failed\n");
           return;
   }
   gpiod_line_settings_set_direction(settings, (iMode == OUTPUT) ? GPIOD_LINE_DIRECTION_OUTPUT : GPIOD_LINE_DIRECTION_INPUT);
   line_cfg = gpiod_line_config_new();
   if (!line_cfg) return;
   gpiod_line_config_add_line_settings(line_cfg, (const unsigned int *)&iPin, 1, settings);
   req_cfg = gpiod_request_config_new();
   gpiod_request_config_set_consumer(req_cfg, CONSUMER);
   lines[iPin] = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
   gpiod_request_config_free(req_cfg);
   gpiod_line_config_free(line_cfg);
   gpiod_line_settings_free(settings);
   gpiod_chip_close(chip);
#endif
} /* pinMode() */

static void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS)
{
char szName[32];

    pinMode(pOBD->iDCPin, OUTPUT);
    if (pOBD->iRSTPin != -1) {
        pinMode(pOBD->iRSTPin, OUTPUT);
        digitalWrite(pOBD->iRSTPin, LOW);
        _delay(100);
        digitalWrite(pOBD->iRSTPin, HIGH);
        _delay(100);
    }
    pOBD->iSpeed = iSpeed;
    pinMode(pOBD->iCSPin, OUTPUT);
    digitalWrite(pOBD->iCSPin, HIGH); // we have to manually control the CS pin
    sprintf(szName, "/dev/spidev%d.%d", iMOSI, iCLK); // SPI channel #
    pOBD->bbi2c.file_i2c = open(szName, O_RDWR);
} /* initSPI() */

// Initialize the I2C bus on Linux
static void I2CInit(BBI2C *pI2C, uint32_t iClock)
{
char filename[32];
int iChannel = pI2C->iSDA;

// Only try to initialize it if it hasn't already been initialized
    if (pI2C->file_i2c == -1) {
        sprintf(filename, "/dev/i2c-%d", iChannel);
        if ((pI2C->file_i2c = open(filename, O_RDWR)) < 0)
        {
                fprintf(stderr, "Failed to open the i2c bus\n");
                return;
        }
    }
} /* I2CInit() */
//
// Test if an I2C address responds
// returns 1 for success, 0 for failure
//
static uint8_t I2CTest(BBI2C *pI2C, uint8_t addr)
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
static int I2CRead(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = read(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CRead() */
//
// Read n bytes from the given address, after setting the register number
//
static int I2CReadRegister(BBI2C *pI2C, uint8_t iAddr, uint8_t u8Register, uint8_t *pData, int iLen)
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
static int I2CWrite(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = write(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CWrite() */

static void SPIWrite(OBDISP *pOBD, uint8_t *pData, int iLen)
{
struct spi_ioc_transfer spi;

    memset(&spi, 0, sizeof(spi));
    spi.tx_buf = (unsigned long)pData;
    spi.len = iLen;
    spi.speed_hz = pOBD->iSpeed;
    spi.bits_per_word = 8;
    ioctl(pOBD->bbi2c.file_i2c, SPI_IOC_MESSAGE(1), &spi);

} /* SPIWrite() */

// Write raw (unfiltered) bytes directly to I2C or SPI
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
  if (pOBD->com_mode == COM_I2C) {// I2C device
      I2CWrite(&pOBD->bbi2c, pOBD->oled_addr, pData, iLen);
  } else { // must be SPI
      digitalWrite(pOBD->iDCPin, (pData[0] != 0));
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP) {
         digitalWrite(pOBD->iCSPin, LOW);
      }
      SPIWrite(pOBD, &pData[1], iLen-1);
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP) {
         digitalWrite(pOBD->iCSPin, HIGH);
      }
      //obdSetDCMode(pOBD, MODE_DATA);
  }
} /* RawWrite() */

static void RawWriteData(OBDISP *pOBD, unsigned char *pData, int iLen)
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
        //SPIWrite(pOBD->bbi2c.file_i2c, &pData[i], 1);
//          digitalWrite(pOBD->iCSPin, HIGH);
//       }
//    } else {
      if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP) {
         digitalWrite(pOBD->iCSPin, LOW);
      }
      SPIWrite(pOBD, pData, iLen);
      if (pOBD->iCSPin != 0xff &&pOBD->chip_type != OBD_CHIP_SHARP) {
         digitalWrite(pOBD->iCSPin, HIGH);
      }
//    }
     //obdSetDCMode(pOBD, MODE_DATA);
  }
} /* RawWriteData() */

