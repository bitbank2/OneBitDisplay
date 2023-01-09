#include <Arduino.h>
//
// OneBitDisplay library demo for AVR platform,
//

#include <OneBitDisplay.h>


// I²C Setup
#define SDA_PIN A4    // change if needed according to your setup
#define SCL_PIN A5    // change if needed according to your setup ()
#define USE_HW_I2C 1  // Use bit banging to get higher speed output
#define OLED_ADDR -1  // let OneBitDisplay find the address of our display

// Screen Setup
#define OLED_SIZE OLED_72x40 // Size of the display
#define RESET_PIN -1         // Using the mcu reset
#define FLIP180    0
#define INVERT     0
#define WIDTH     72                   // Helper to calculate te buffer size according to the screen width in pixels
#define HEIGHT    40                   // Helper to calculate te buffer size according to the screen height in pixels
#define BUFFER_SIZE (WIDTH*HEIGHT) / 8 // the actual size of the buffer needed for your screen

// Globals
OBDISP obd;
static uint8_t ucBuffer[BUFFER_SIZE] = {0};

void setup()
{
    
    int rc;
    rc = obdI2CInit(&obd, OLED_SIZE, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L);       // Standard HW I2C bus at 400Khz
    
    if (rc != OLED_NOT_FOUND)
    {
        obdSetBackBuffer(&obd, ucBuffer);

        char *msgs[] =
        {
          (char *)"SSD1306 @ 0x3C",
          (char *)"SSD1306 @ 0x3D",
          (char *)"SH1106 @ 0x3C",
          (char *)"SH1106 @ 0x3D"
        };

        // Display address and chipset
        obdFill(&obd, 0x00, 1);
        obdSetTextWrap(&obd, 1 );
        obdWriteString(&obd, 0, 0, 0, (char *)"OLED found:", FONT_SMALL, OBD_BLACK, 1);
        obdWriteString(&obd, 0, 0, 8, msgs[rc], FONT_SMALL, OBD_BLACK, 1);
        delay(3000);

        // Display a rectangle for demo
        obdFill(&obd, 0x00, 1);
        obdRectangle(&obd, 0, 0, obd.width-1, obd.height-1, OBD_BLACK, 0);
        obdDumpBuffer(&obd, ucBuffer);
        delay(3000);
    }
}

void loop()
{
    int i, x, y;
    
    obdFill(&obd, 0x00, 1);
    obdWriteString(&obd, 0, 0, 0,(char *)"OLED Demo", FONT_NORMAL, OBD_BLACK, 1);
    obdSetTextWrap(&obd, 1 );
    obdWriteString(&obd, 0, 0, -1,(char *)"Written by Larry Bank", FONT_SMALL, OBD_BLACK, 1);
    obdWriteString(&obd, 0, 0, -1,(char *)"**Demo**", FONT_NORMAL, OBD_BLACK, 1);
    obdWriteString(&obd, 0, 9, -1,(char *)"for AVR", FONT_NORMAL, OBD_BLACK, 1);

    delay(2000);
    
    obdFill(&obd, 0x00, 1);

    for (i = 0; i < 30; i++)
    {
        x = random(obd.width);
        y = random(obd.height);
        obdSetPixel(&obd, x, y, OBD_BLACK, 1);
        obdDumpBuffer(&obd, ucBuffer);
    }

    delay(2000);
}