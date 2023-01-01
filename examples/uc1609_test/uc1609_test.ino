//
// OneBitDisplay library GFX demo
//
#include <OneBitDisplay.h>
#include "uc1609.h" // demo bitmap

#define RESET_PIN 9
#define DC_PIN 8
#define CS_PIN 10
#define MOSI_PIN -1
#define SCK_PIN -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0

#ifndef __AVR__
#define USE_BACKBUFFER
#endif

#ifdef USE_BACKBUFFER
uint8_t ucBackBuffer[1536];
#endif

// The OBDISP structure
// There is no limit to the number of simultaneous displays which can be controlled by OneBitDisplay 
OBDISP obd;

void setup() {
int rc;
  Serial.begin(115200);
  
//void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int iBitBang, int32_t iSpeed);
  obdSPIInit(&obd, LCD_UC1609, DC_PIN, CS_PIN, RESET_PIN, MOSI_PIN, SCK_PIN, -1, 0, 0, 0, 4000000);
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0,0,0,(char *)"UC1701 192x64 LCD Demo", FONT_8x8, OBD_BLACK, 1);
  delay(4000);
  rc = obdLoadBMP(&obd, (uint8_t *)uc1609, 0, 0, OBD_BLACK, OBD_WHITE);
  if (rc != 0)
     Serial.println("Error displaying bitmap");
  delay(4000);
#ifdef USE_BACKBUFFER
  obdSetBackBuffer(&obd, ucBackBuffer);
#endif
} /* setup() */

#ifdef USE_BACKBUFFER
#define DRAW_ELLIPSES
#define DRAW_RECTS
#define DRAW_TEXT
#endif

void loop() {
  int i, x, y, x2, y2, r1, r2;
  uint8_t ucColor;
  unsigned long ulTime;
  char szTemp[32];
  
#ifdef DRAW_ELLIPSES
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"Ellipses", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd, OBD_WHITE, 1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    x = random(192);
    y = random(64);
    r1 = random(64);
    r2 = random(32);
    obdEllipse(&obd, x, y, r1, r2, OBD_BLACK, 0);
    obdDumpBuffer(&obd, NULL);    
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"100 frames drawn", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "%d FPS", 100000000 / (ulTime));
  obdWriteString(&obd, 0, 0, 2, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"Filled Ellipses", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd, OBD_WHITE, 1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    x = random(192);
    y = random(64);
    r1 = random(64);
    r2 = random(32);
    ucColor = random(2);
    obdEllipse(&obd, x, y, r1, r2, ucColor, 1);
    obdDumpBuffer(&obd, NULL);
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, 0, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"100 frames drawn", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "%d FPS", 100000000 / (ulTime));
  obdWriteString(&obd, 0, 0, 2, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);

#endif // DRAW_ELLIPSES

#ifdef DRAW_RECTS
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"Rectangles", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd, OBD_WHITE, 1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    x = random(192);
    y = random(64);
    x2 = random(192);
    y2 = random(64);
    obdRectangle(&obd, x, y, x2, y2, OBD_BLACK, 0);
    obdDumpBuffer(&obd, NULL);
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"100 frames drawn", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "%d FPS", 100000000 / (ulTime));
  obdWriteString(&obd, 0, 0, 2, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"Filled Rects", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd, OBD_WHITE, 1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    x = random(192);
    y = random(64);
    x2 = random(192);
    y2 = random(64);
    ucColor = random(2);
    obdRectangle(&obd, x, y, x2, y2, ucColor, 1);
    obdDumpBuffer(&obd, NULL);
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"100 frames drawn", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "%d FPS", 100000000 / (ulTime));
  obdWriteString(&obd, 0, 0, 2, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);
#endif // DRAW_RECTS
#ifdef DRAW_TEXT
  obdFill(&obd,OBD_WHITE,1);
  obdWriteString(&obd,0,0,0,(char *)"Small Font", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd,OBD_WHITE,1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    char szTemp[34];
     szTemp[32] = 0;
     x = i;
     while (x >= 26)
        x -= 26;
     for (y=0; y<8; y++)
     {
        for (x2=0; x2<32; x2++)
        {
           szTemp[x2] = 'A' + x;
           x++;
           if (x >= 26) x -= 26;
        }
        obdWriteString(&obd, 0,0,y,szTemp, FONT_6x8, OBD_BLACK, 1);
     } // for y
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"16800 chars", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);
  obdFill(&obd,OBD_WHITE,1);
  obdWriteString(&obd,0,0,0,(char *)"Normal Font", FONT_8x8, OBD_BLACK, 1);
  delay(2000);
  obdFill(&obd,OBD_WHITE,1);
  ulTime = micros();
  for (i=0; i<100; i++)
  {
    char szTemp[34];
     szTemp[24] = 0;
     x = i;
     while (x >= 26)
        x -= 26;
     for (y=0; y<8; y++)
     {
        for (x2=0; x2<24; x2++)
        {
           szTemp[x2] = 'A' + x;
           x++;
           if (x >= 26) x -= 26;
        }
        obdWriteString(&obd, 0,0,y,szTemp, FONT_8x8, OBD_BLACK, 1);
     } // for y
  }
  ulTime = micros() - ulTime;
  obdFill(&obd, OBD_WHITE, 1);
  obdWriteString(&obd, 0, 0, 0, (char *)"12800 chars", FONT_8x8, OBD_BLACK, 1);
  sprintf(szTemp, "in %d ms", (int)(ulTime/1000));
  obdWriteString(&obd, 0, 0, 1, szTemp, FONT_8x8, OBD_BLACK, 1);
  delay(4000);
#endif // DRAW_TEXT
} /* loop() */
