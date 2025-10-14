#include <OneBitDisplay.h>

OBDISP obd;
#define FLIP180 0
#define INVERT 0
#define BITBANG 1
#define DC_PIN 22
#define CS_PIN 3
#define RESET_PIN 1
#define MOSI_PIN 19
#define CLK_PIN 23
#define LED_PIN 18
void setup() {
  // put your setup code here, to run once:
//void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int bBitBang, int32_t iSpeed)
  obdSPIInit(&obd, LCD_UC1701, DC_PIN, CS_PIN, RESET_PIN, MOSI_PIN, CLK_PIN, LED_PIN, FLIP180, INVERT, BITBANG, 0);
  obdFill(&obd, OBD_WHITE, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  obdWriteString(&obd, 0, 0, 0, (char *)"Hello World!", FONT_8x8, OBD_BLACK, 1);
  while (1)
  {
    
  };
  
}
