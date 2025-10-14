//
// OneBitDisplay library multi-display demo
//
// Demonstrates how to initialize and use multiple displays
//
#include <OneBitDisplay.h>

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are reversed because I did straight-through wiring for my SSD1306
// and it has the 4-pin header as GND,VCC,SCL,SDA, but the GROVE connector is
// GND,VCC,SDA,SCL
#define GROVE_SDA_PIN 32
#define GROVE_SCL_PIN 26
// These are the pin numbers for the M5Stack Atom default I2C
#define SDA_PIN 21
#define SCL_PIN 22
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let ss_oled figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 0

// Change these if you're using different OLED displays
#define MY_OLED1 OLED_128x64
#define MY_OLED2 OLED_64x32

// 2 copies of the SSOLED structure. Each structure is about 56 bytes
// There is no limit to the number of simultaneous displays which can be controlled by ss_oled 
OBDISP obd[2];

void setup() {
char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
int rc;
// The I2C SDA/SCL pins set to -1 means to use the default Wire library
// If pins were specified, they would be bit-banged in software
// This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
// The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
//    that can be configured as either SPI or I2C
//
// obdI2CInit(OBDISP *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)

rc = obdI2CInit(&obd[0], MY_OLED1, OLED_ADDR, FLIP180, INVERT, 1, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND)
  {
    obdFill(&obd[0], OBD_WHITE, 1);
    obdWriteString(&obd[0], 0,0,0,msgs[rc], FONT_8x8, OBD_BLACK, 1);
    obdWriteString(&obd[0], 0,8,3,(char *)"Display", FONT_16x16, OBD_BLACK, 1);
    obdWriteString(&obd[0], 0,56,6,(char *)"0", FONT_16x16, OBD_BLACK, 1);
  }
rc = obdI2CInit(&obd[1], MY_OLED2, OLED_ADDR, FLIP180, INVERT, 0, GROVE_SDA_PIN, GROVE_SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND)
  {
    obdFill(&obd[1], OBD_WHITE, 1);
    obdSetTextWrap(&obd[1], 1);
    obdWriteString(&obd[1], 0,0,0,msgs[rc], FONT_6x8, OBD_BLACK, 1);
    obdWriteString(&obd[1], 0,4,2,(char *)"Display", FONT_8x8, OBD_BLACK, 1);
    obdWriteString(&obd[1], 0,28,3,(char *)"1", FONT_8x8, OBD_BLACK, 1);
  }
} /* setup() */

void loop() {
  // put your main code here, to run repeatedly:

} /* loop() */
