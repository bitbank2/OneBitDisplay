//
// OneBitDisplay library simple demo
//
// Demonstrates how to initialize and use a few functions of the library
// If your MCU has enough RAM, enable the backbuffer to see a demonstration
// of the speed difference between drawing directly on the display versus
// deferred rendering, followed by a "dump" of the memory to the display
//
#include <OneBitDisplay.h>
#include "FreeSerif12pt7b.h"

static uint8_t ucBackBuffer[1024];

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom Grove port I2C (reversed SDA/SCL for straight through wiring)
#define SDA_PIN 32
#define SCL_PIN 26
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let OneBitDisplay figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 1

// Change these if you're using a different OLED display
#define MY_OLED OLED_128x64
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
//#define MY_OLED OLED_64x32
//#define OLED_WIDTH 64
//#define OLED_HEIGHT 32

OBDISP obd;

void setup() {
int rc;
// The I2C SDA/SCL pins set to -1 means to use the default Wire library
// If pins were specified, they would be bit-banged in software
// This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
// The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
//    that can be configured as either SPI or I2C
//
// obdI2CInit(OBDISP *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)

rc = obdI2CInit(&obd, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 800000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND)
  {
    char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
    obdFill(&obd, OBD_WHITE, 1);
    obdWriteString(&obd, 0,0,0,msgs[rc], FONT_8x8, OBD_BLACK, 1);
    obdSetBackBuffer(&obd, ucBackBuffer);
    delay(2000);
  }
} /* setup() */

void loop() {
int y;
char szTemp[32];
unsigned long ms;

    obdFill(&obd, OBD_WHITE, 0);
    ms = micros();
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, 16, (char *)"Hello World",OBD_BLACK);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, 16+FreeSerif12pt7b.yAdvance, (char *)"Fast Perf!", OBD_BLACK);
    ms = micros() - ms;
    obdDumpBuffer(&obd, NULL);
    sprintf(szTemp, "rendered in %d us", (int)ms);
    obdWriteString(&obd, 0,0,7,szTemp, FONT_6x8, OBD_BLACK, 1);
    delay(4000);
    
  for (y=-30; y<80; y++)
  {
    obdFill(&obd, OBD_BLACK, 0);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, y, (char *)"Hello World", OBD_WHITE);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, y+FreeSerif12pt7b.yAdvance, (char *)"Fast Perf!", OBD_WHITE);
    obdDumpBuffer(&obd, NULL);
  }
  for (y=79; y>=-30; y--)
  {
    obdFill(&obd, OBD_WHITE, 0);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, y, (char *)"Hello World", OBD_BLACK);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, y+FreeSerif12pt7b.yAdvance, (char *)"Fast Perf!", OBD_BLACK);
    obdDumpBuffer(&obd, NULL);
  }
} /* loop() */
