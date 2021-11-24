//
//  main.c
//  OneBitDisplay library test program
//  Demonstrates many of the features for OLED displays
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <armbianio.h>

#include "../../src/OneBitDisplay.cpp"

volatile int iStop = 0;

//#define OLED_TYPE OLED_132x64
//#define OLED_TYPE LCD_UC1609
#define OLED_TYPE LCD_UC1701
#define FLIP180 0
#define INVERT 0
#define DC_PIN 22
#define CS_PIN -1
//#define CS_PIN 24
#define RESET_PIN 13
#define SPEED 4000000
#define BITBANG 0
// LED on GPIO 12
#define LED_PIN 32
OBDISP obd;
static uint8_t ucBuffer[1024];

void my_handler(int signal)
{
   iStop = 1;
} /* my_handler() */

int main(int argc, const char *argv[])
{
int i;
struct sigaction sigIntHandler;
//size_t ret;

// Set CTRL-C signal handler
  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  i = AIOInitBoard("Raspberry Pi");
  if (i == 0) // problem
  {
     printf("Error in AIOInit(); check if this board is supported\n");
     return 0;
  }

//void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int bBitBang, int32_t iSpeed)  
    obdSPIInit(&obd, OLED_TYPE, DC_PIN, CS_PIN, RESET_PIN, -1, -1, LED_PIN, FLIP180, INVERT, BITBANG, SPEED);
    obdSetBackBuffer(&obd, ucBuffer);
    // Create some simple content
    obdFill(&obd, 0, 1);
    obdSetContrast(&obd, 63); // white on black requires max contrast to be visible
    obdBacklight(&obd, 1);
    obdWriteString(&obd,0,0,0,"OneBitDisplay", FONT_8x8, 0, 1);
    while (!iStop) {
       obdFill(&obd, 0, 1);
       for (int i=0; i<128; i++) {
          obdDrawLine(&obd, i, 0, 127-i, 63, 1, 1);
       }
       for (int i=0; i<64; i++) {
          obdDrawLine(&obd, 127, i, 0, 63-i, 1, 1);
       }
    }
//    obdEllipse(&obd, 320, 240+64, 150,100, 0, 1);
//    obdRectangle(&obd, 300, 240+32, 340, 240+96, 1, 1);
    obdFill(&obd, 0, 1);
    obdBacklight(&obd, 0);
    AIOShutdown();
    return 0;
} /* main() */
