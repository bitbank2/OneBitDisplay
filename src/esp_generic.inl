//
// esp-idf I/O wrapper functions
//
#ifndef __OBD_IO__
#define __OBD_IO__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "rom/ets_sys.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#ifdef VSPI_HOST
#define ESP32_SPI_HOST VSPI_HOST
#else
#define ESP32_SPI_HOST SPI2_HOST
#endif // VSPI_HOST

#define I2C_MASTER_NUM              0
static uint8_t u8SDA_Pin, u8SCL_Pin;
static int iDelay = 1;
#define DISABLED 0
#define INPUT 1
#define INPUT_PULLUP 2
#define OUTPUT 3
#define INPUT_PULLDOWN 4

#define MAX_CACHE 192
static uint8_t u8Cache[MAX_CACHE]; // for faster character drawing
static volatile uint8_t u8End = 0;
static uint8_t u8Temp[40]; // for stretched character drawing
static spi_device_interface_config_t devcfg;
static spi_bus_config_t buscfg;
static spi_device_handle_t spi;


static int I2CWrite(BBI2C *pI2C, unsigned char iAddr, unsigned char *pData, int iLen);

//static unsigned long micros(void)
//{
//    return (unsigned long)(esp_timer_get_time());
//}

static void delayMicroseconds(uint32_t us)
{
    ets_delay_us(us);
}

static void _delay(int ms)
{
    if (ms >= 10) {
        vTaskDelay(ms/10);
    }
    delayMicroseconds((ms % 10) * 1000);
} /* _delay() */

static void pinMode(int iPin, int iMode)
{
    gpio_config_t io_conf = {};

    gpio_reset_pin((gpio_num_t)iPin);
    if (iMode == DISABLED) return;
    io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL << iPin);
    io_conf.pull_down_en = (gpio_pulldown_t)(iMode == INPUT_PULLDOWN); // pull-down mode
    io_conf.pull_up_en = (gpio_pullup_t)(iMode == INPUT_PULLUP); // pull-up mode
    if (iMode == OUTPUT) {
        io_conf.mode = GPIO_MODE_OUTPUT;
    } else { // must be input
        io_conf.mode = GPIO_MODE_INPUT;
    }
    gpio_config(&io_conf); //configure GPIO with the given settings
} /* pinMode() */

static void digitalWrite(uint8_t pin, uint8_t value)
{
    gpio_set_level((gpio_num_t)pin, value);
} /* digitalWrite() */

static uint8_t SDA_READ(void)
{
    return gpio_get_level((gpio_num_t)u8SDA_Pin);
}
static void SDA_HIGH(void)
{
    pinMode(u8SDA_Pin, INPUT_PULLDOWN);
}
static void SDA_LOW(void)
{
    pinMode(u8SDA_Pin, OUTPUT);
    gpio_set_level((gpio_num_t)u8SDA_Pin, 0);
}
static void SCL_HIGH(void)
{
    pinMode(u8SCL_Pin, INPUT_PULLDOWN);
}
static void SCL_LOW(void)
{
    pinMode(u8SCL_Pin, OUTPUT);
    gpio_set_level((gpio_num_t)u8SCL_Pin, 0);
}

// Transmit a byte and read the ack bit
// if we get a NACK (negative acknowledge) return 0
// otherwise return 1 for success
//

static int i2cByteOut(uint8_t b)
{
uint8_t i, ack;

    for (i=0; i<8; i++) {
//    delayMicroseconds(iDelay);
        if (b & 0x80)
            SDA_HIGH(); // set data line to 1
        else
            SDA_LOW(); // set data line to 0
        b <<= 1;
//    delayMicroseconds(iDelay);
        SCL_HIGH(); // clock high (slave latches data)
        delayMicroseconds(iDelay);
        SCL_LOW(); // clock low
        delayMicroseconds(iDelay);
    } // for i
    //delayMicroseconds(iDelay);
    // read ack bit
    SDA_HIGH(); // set data line for reading
    //delayMicroseconds(iDelay);
    SCL_HIGH(); // clock line high
    delayMicroseconds(iDelay); // DEBUG - delay/2
    ack = SDA_READ();
    //delayMicroseconds(iDelay);
    SCL_LOW(); // clock low
    delayMicroseconds(iDelay); // DEBUG - delay/2
    SDA_LOW(); // data low
    return (ack == 0); // a low ACK bit means success
} /* i2cByteOut() */
//
// Receive a byte and read the ack bit
// if we get a NACK (negative acknowledge) return 0
// otherwise return 1 for success
//
static uint8_t i2cByteIn(uint8_t bLast)
{
uint8_t i;
uint8_t b = 0;

     SDA_HIGH(); // set data line as input
     for (i=0; i<8; i++)
     {
         delayMicroseconds(iDelay); // wait for data to settle
         SCL_HIGH(); // clock high (slave latches data)
         delayMicroseconds(iDelay);
         b <<= 1;
         if (SDA_READ() != 0) // read the data bit
           b |= 1; // set data bit
         SCL_LOW(); // clock low
     } // for i
     if (bLast)
        SDA_HIGH(); // last byte sends a NACK
     else
     SDA_LOW();
     //     delayMicroseconds(iDelay);
          SCL_HIGH(); // clock high
          delayMicroseconds(iDelay);
          SCL_LOW(); // clock low to send ack
          delayMicroseconds(iDelay);
     //     SDA_HIGH();
          SDA_LOW(); // data low
       return b;
} /* i2cByteIn() */
//
// Send I2C STOP condition
//
static void i2cEnd(void)
{
   SDA_LOW(); // data line low
   delayMicroseconds(iDelay);
   SCL_HIGH(); // clock high
   delayMicroseconds(iDelay);
   SDA_HIGH(); // data high
   delayMicroseconds(iDelay);
} /* i2cEnd() */

static int i2cBegin(uint8_t addr, uint8_t bRead)
{
   int rc;
//   SCL_HIGH();
//   delayMicroseconds(iDelay);
   SDA_LOW(); // data line low first
   delayMicroseconds(iDelay);
   SCL_LOW(); // then clock line low is a START signal
   addr <<= 1;
   if (bRead)
      addr++; // set read bit
   rc = i2cByteOut(addr); // send the slave address and R/W bit
   return rc;
} /* i2cBegin() */

static void BBI2CWrite(uint8_t addr, uint8_t *pData, int iLen)
{
uint8_t b;
int rc;
     
    i2cBegin(addr, 0);
    rc = 1;
    while (iLen && rc == 1)
    {
       b = *pData++;
       rc = i2cByteOut(b);
       if (rc == 1) // success
       {
          iLen--;
       }
    } // for each byte
    i2cEnd();
//return (rc == 1) ? (iOldLen - iLen) : 0; // 0 indicates bad ack from sending a byte
} /* BBI2CWrite() */

static int BBI2CRead(uint8_t addr, uint8_t *pData, int iLen)
{
    i2cBegin(addr, 1);
    while (iLen--)
    {
       *pData++ = i2cByteIn(iLen == 0);
    } // for each byte
    i2cEnd();
    return 1;
} /* BBI2CRead() */

static int I2CTest(BBI2C *pI2C, uint8_t addr)
{
     int response = 0;
     
        if (pI2C->bWire) {
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      if (cmd == NULL) {
         // ESP_LOGE("OneBitDisplay", "insufficient memory for I2C transaction");
      }
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
      i2c_master_stop(cmd);
      esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd);
      response = (ret == ESP_OK);
        } else {
        if (i2cBegin(addr, 0)) // try to write to the given address
        {
           response = 1;
        }
        i2cEnd();
      }
     return response;
     } /* I2CTest() */
//
// Initialize the Wire library on the given SDA/SCL GPIOs
//
static void I2CInit(BBI2C *pI2C, unsigned int iClock)
{
    if (!pI2C->bWire) {
        u8SDA_Pin = pI2C->iSDA;
        u8SCL_Pin = pI2C->iSCL;
//    if (iSpeed >= 400000) iDelay = 1;
//    else if (iSpeed >= 100000) iDelay = 10;
//    else iDelay = 20;
    } else if (pI2C->iSDA != 0xff) { // -1 indicates that I2C is already initialized
        i2c_config_t conf;
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = pI2C->iSDA;
        conf.scl_io_num = pI2C->iSCL;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = iClock;
        conf.clk_flags = 0; 
        ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
        ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
        pI2C->file_i2c = I2C_NUM_0; // I2C handle used
    }
    return;
} /* I2CInit() */

static int I2CWrite(BBI2C *pI2C, unsigned char iAddr, unsigned char *pData, int iLen)
{

    if (!pI2C->bWire) {
        BBI2CWrite(iAddr, pData, iLen);
        return 1;
    } else {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
       // ESP_LOGE("bb_epdiy", "insufficient memory for I2C transaction");
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (iAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, pData, iLen, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return (ret == ESP_OK);
    }
} /* I2CWrite() */

static int I2CRead(BBI2C *pI2C, unsigned char iAddr, unsigned char *pData, int iLen)
{
int i = 0;

    if (!pI2C->bWire) {
        i = I2CRead(pI2C, iAddr, pData, iLen);
    } else {
    esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
       // ESP_LOGE("epdiy", "insufficient memory for I2C transaction");
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (iAddr << 1) | I2C_MASTER_READ, true);
    if (iLen > 1) {
        i2c_master_read(cmd, pData, iLen - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, pData + iLen - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret == ESP_OK) {
        i = iLen;
    }
    i2c_cmd_link_delete(cmd);
    }
    return i;
} /* I2CRead() */

static int I2CReadRegister(BBI2C *pI2C, unsigned char iAddr, unsigned char u8Register, unsigned char *pData, int iLen)
{
    if (pI2C->bWire) {
        I2CWrite(pI2C, iAddr, &u8Register, 1);
        I2CRead(pI2C, iAddr, pData, iLen);
    } else {
        BBI2CWrite(iAddr, &u8Register, 1);
        BBI2CRead(iAddr, pData, iLen);
    }
    return iLen;
} /* I2CReadRegister() */

// Initialize SPI
void initSPI(OBDISP *pOBD, int iSpeed, int iMOSI, int iCLK, int iCS)
{
    esp_err_t ret;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.miso_io_num = -1; //iMISO;
    buscfg.mosi_io_num = iMOSI;
    buscfg.sclk_io_num = iCLK;
    buscfg.max_transfer_sz=4096;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
    //Initialize the SPI bus
    ret=spi_bus_initialize(ESP32_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret==ESP_OK);

    memset(&devcfg, 0, sizeof(devcfg));
    devcfg.clock_speed_hz = iSpeed;
    devcfg.mode = 0;
    devcfg.spics_io_num = -1;            //CS pin, set to -1 to disable since we handle it outside of the master driver
    devcfg.queue_size = 2;                          //We want to be able to queue 2 transactions at a time
// These callbacks currently don't do anything
//    devcfg.pre_cb = spi_pre_transfer_callback;  //Specify pre-transfer callback to handle D/C line
//    devcfg.post_cb = spi_post_transfer_callback;
//    devcfg.flags = SPI_DEVICE_NO_DUMMY; // allow speeds > 26Mhz
    devcfg.flags = SPI_DEVICE_HALFDUPLEX; // this disables SD card access
    //Attach the LCD/OLED to the SPI bus
    ret=spi_bus_add_device(ESP32_SPI_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);

} /* initSPI() */

void espSPIWrite(uint8_t *pBuf, int iLen)
{
esp_err_t ret;
static spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    while (iLen) {
        int l = iLen;
        if (l > 4000) { // transmit maximum length (half duplex mode)
             l = 4000;
        }
        t.length=l*8;  // length in bits
        t.rxlength = 0;
        t.tx_buffer=pBuf;
        ret=spi_device_polling_transmit(spi, &t);  //Transmit!
        assert(ret==ESP_OK);            //Should have had no issues.
        iLen -= l;
        pBuf += l;
      } // while (iLen)
} /* espSPIWrite() */

static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
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
//    if (pOBD->bBitBang) {
//        SPI_BitBang(pOBD, &pData[1], iLen-1, pOBD->iMOSIPin, pOBD->iCLKPin);
//    else
        espSPIWrite(&pData[1], iLen-1);
//    }
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP) {
        digitalWrite(pOBD->iCSPin, HIGH);
    }
    if (pOBD->iDCPin != 0xff && pData[0] == 0) { // was command mode, set back to data mode
        digitalWrite(pOBD->iDCPin, HIGH);
    }
  } else { // must be I2C
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
  if (pOBD->com_mode == COM_SPI) // we're writing to SPI, treat it differently
  {
    if (pOBD->iDCPin != 0xff)
      digitalWrite(pOBD->iDCPin, HIGH); // data mode
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
      digitalWrite(pOBD->iCSPin, LOW);
    espSPIWrite(pData, iLen);
    if (pOBD->iCSPin != 0xff && pOBD->chip_type != OBD_CHIP_SHARP)
      digitalWrite(pOBD->iCSPin, HIGH);
  } else { // Must be I2C
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
#endif // __OBD_IO__
