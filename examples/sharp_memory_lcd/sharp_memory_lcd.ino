// Sharp Memory LCD (144x168)

#include <OneBitDisplay.h>

OBDISP obd;
#define FLIP180 0
#define INVERT 0
#define BITBANG 0
#define DC_PIN -1
#define CS_PIN 10
#define RESET_PIN -1
#define MOSI_PIN -1
#define CLK_PIN -1
#define LED_PIN -1
uint8_t ucBackBuf[400*240/8];

void setup() {
  obdSPIInit(&obd, SHARP_400x240, DC_PIN, CS_PIN, RESET_PIN, MOSI_PIN, CLK_PIN, LED_PIN, FLIP180, INVERT, BITBANG, 8000000L);
  obdSetBackBuffer(&obd, ucBackBuf);
  obdFill(&obd, OBD_WHITE, 0);
  obdWriteString(&obd,0,0,0,(char *)"Sharp Memory LCD Demo!", FONT_16x16, OBD_BLACK, 0);
  obdDumpBuffer(&obd, NULL);
}

void loop() {
  int i;
  
  while (1)
  {
    for (i=1; i<100; i++)
    {
      obdEllipse(&obd, 200, 140, i, i, OBD_BLACK, 0); // circle, not filled
      obdDumpBuffer(&obd, NULL);
    }
    for (i=99; i>=1; i--)
    {
      obdEllipse(&obd, 200, 140, i, i, OBD_WHITE, 0); // circle, not filled
      obdDumpBuffer(&obd, NULL);
    }
  }  
}
