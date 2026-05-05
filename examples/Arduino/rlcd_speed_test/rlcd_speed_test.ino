//
// Waveshare 4.2" RLCD 400x300 speed test
// written by Larry Bank
// May 5, 2026
//

#include <OneBitDisplay.h>

OBDISP obd;
#define FLIP180 0
#define INVERT 0
#define BITBANG 0
#define DC_PIN 5
#define CS_PIN 40
#define RESET_PIN 41
#define MOSI_PIN 12
#define CLK_PIN 11
#define LED_PIN -1
uint8_t ucBackBuf[(400/8)*308];

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  obdSPIInit(&obd, LCD_ST7305B, DC_PIN, CS_PIN, RESET_PIN, MOSI_PIN, CLK_PIN, LED_PIN, FLIP180, INVERT, BITBANG, 40000000L);
  obdSetBackBuffer(&obd, ucBackBuf);
  obdFill(&obd, OBD_WHITE, 0);
  obdWriteString(&obd,0,8,16,(char *)"OneBitDisplay RLCD Demo!", FONT_16x16, OBD_BLACK, 0);
  obdDumpBuffer(&obd, NULL);
} /* setup() */

void loop() {
  int i;
  long l;
  char szTemp[32];

  while (1)
  {
    for (i=1; i<100; i++)
    {
      obdEllipse(&obd, 200, 140, i, i, OBD_BLACK, 0); // circle, not filled
      obdDumpBuffer(&obd, NULL);
    }
    l = millis();
    for (i=99; i>=1; i--)
    {
      obdEllipse(&obd, 200, 140, i, i, OBD_WHITE, 0); // circle, not filled
      obdDumpBuffer(&obd, NULL);
    }
    l = millis() - l;
    sprintf(szTemp, "Avg fps %d ", 99000/(int)l);
    obdWriteString(&obd,0,8,obd.native_height-16, szTemp, FONT_12x16, OBD_BLACK, 0);
  }  
} /* loop() */
