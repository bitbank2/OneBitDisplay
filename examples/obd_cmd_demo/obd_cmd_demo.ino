//
// OneBitDisplay command example
// shows how to create and play back a buffer of bytewise display commands
// written by Larry Bank 1/23/2022
//
#include <OneBitDisplay.h>
OBDISP obd, obdCMD;
uint8_t ucBuffer[1024], ucBackBuffer[1024];
// 16x16 X image
uint8_t ucSprite[32] = {0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x08,
                        0x08,0x10,0x04,0x20,0x02,0x40,0x01,0x80,
                        0x01,0x80,0x02,0x40,0x04,0x20,0x08,0x10,
                        0x10,0x08,0x20,0x04,0x40,0x02,0x80,0x01};
void setup() {
  int rc;
  char szTemp[32];
//  int obdI2CInit(OBDISP *pOBD, int iType, int iAddr, int bFlip, int bInvert, int bWire, int iSDAPin, int iSCLPin, int iResetPin, int32_t iSpeed);
  rc = obdI2CInit(&obd, OLED_128x64, -1, 0, 0, 1, -1, -1, -1, 800000);
  obdSetBackBuffer(&obd, ucBackBuffer);
  obdFill(&obd, OBD_WHITE, 1);
  memset(&obdCMD, 0, sizeof(obdCMD)); // ready for commands
  obdSetBackBuffer(&obdCMD, ucBuffer); // give it a memory space to save the commands
  
  obdWriteString(&obd, 0,0,0, (char *)"OBD Command test", FONT_8x8, OBD_BLACK, 1);
  obdWriteString(&obd, 0,0,1, (char *)"Executing...", FONT_8x8, OBD_BLACK, 1);
  obdFill(&obdCMD, OBD_WHITE, 1);
  obdWriteString(&obdCMD, 0,0,0,(char *)"The OBD lib functions", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&obdCMD, 0,0,1,(char *)"become byte codes", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&obdCMD, 0,0,2,(char *)"written into memory", FONT_6x8, OBD_BLACK, 1);
  obdWriteString(&obdCMD, 0,0,3,(char *)"and then played back", FONT_6x8, OBD_BLACK, 1);
  obdEllipse(&obdCMD, 32, 48, 31, 15, OBD_BLACK, 1);
  obdEllipse(&obdCMD, 32, 48, 15, 12, OBD_WHITE, 1);
  obdDrawSprite(&obdCMD, ucSprite, 16, 16, 2, 96,40, 1);
//  obdEllipse(&obdCMD, 96, 48, 31, 15, 1, 1);
//  obdEllipse(&obdCMD, 96, 48, 15, 12, 0, 1);
  sprintf(szTemp, "%d bytes generated", obdGetCommandLen(&obdCMD));
  obdWriteString(&obd, 0,0,2, szTemp, FONT_6x8, OBD_BLACK, 1);
  delay(4000);
  obdExecCommands(ucBuffer, obdGetCommandLen(&obdCMD), &obd, 0);
  obdDumpBuffer(&obd, NULL);
} /* setup() */

void loop() {
  // put your main code here, to run repeatedly:

}
