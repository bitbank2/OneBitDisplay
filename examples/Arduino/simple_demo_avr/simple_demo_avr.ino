//
// OneBitDisplay library demo for AVR platform,
// without line drawing function (saving of 1K RAM)
//
#include <OneBitDisplay.h>

OBDISP obd;
#define SDA_PIN -1
#define SCL_PIN -1
// no reset pin needed
#define RESET_PIN -1
// let OneBitDisplay find the address of our display
#define OLED_ADDR -1
#define FLIP180 0
#define INVERT 0
// Use the default Wire library
#define USE_HW_I2C 1
void setup()
{
    int rc;
    rc = obdI2CInit(&obd, OLED_128x64, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L);       // Standard HW I2C bus at 400Khz

    if (rc != OLED_NOT_FOUND)
    {
        char *msgs[] =
        {
          (char *)"SSD1306 @ 0x3C",
          (char *)"SSD1306 @ 0x3D",
          (char *)"SH1106 @ 0x3C",
          (char *)"SH1106 @ 0x3D"
        };

        obdFill(&obd, 0, 1);
        obdWriteString(&obd, 0, 0, 0, (char *)"OLED found:", FONT_8x8, 0, 1);
        obdWriteString(&obd, 0, 10, 2, msgs[rc], FONT_8x8, 0, 1);
        delay(3000);
    }
}

void loop()
{
    int i, x, y;

    obdFill(&obd, 0, 1);
    obdWriteString(&obd, 0, 28, 0,(char *)"OLED Demo", FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1,(char *)"Written by Larry Bank", FONT_6x8, 1, 1);
    obdWriteString(&obd, 0, 0, 3,(char *)"**Demo**", FONT_16x16, 0, 1);
    obdWriteString(&obd, 0, 9, 6,(char *)"for AVR", FONT_16x16, 0, 1);

    delay(2000);
    obdFill(&obd, 0, 1);

    for (i = 0; i < 1000; i++)
    {
        x = random(128);
        y = random(64);
        obdSetPixel(&obd, x, y, 1, 1);
    }

    delay(2000);
}
