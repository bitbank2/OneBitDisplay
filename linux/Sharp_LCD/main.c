//
//  main.c
//  OneBitDisplay library test program
//  Sharp Memory LCD Demo
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

#define DISPLAY_TYPE SHARP_400x240
#define FLIP180 0
#define INVERT 0
#define DC_PIN -1
#define CS_PIN 16
#define DISP_PIN 18
#define EXTCOM_PIN 22
#define RESET_PIN -1
#define SPEED 4000000
#define BITBANG 0
#define LED_PIN -1
OBDISP obd;
static uint8_t ucBuffer[12000];

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

  printf("Sharp Memory LCD demo\n");
  printf("Press Ctrl-C to quit\n");

  i = AIOInitBoard("Raspberry Pi");
  if (i == 0) // problem
  {
     printf("Error in AIOInit(); check if this board is supported\n");
     return 0;
  }
  AIOAddGPIO(DISP_PIN, OUTPUT); // enable display
  AIOWriteGPIO(DISP_PIN, HIGH);
  AIOAddGPIO(EXTCOM_PIN, OUTPUT);

//void obdSPIInit(OBDISP *pOBD, int iType, int iDC, int iCS, int iReset, int iMOSI, int iCLK, int iLED, int bFlip, int bInvert, int bBitBang, int32_t iSpeed)  
    obdSPIInit(&obd, DISPLAY_TYPE, DC_PIN, CS_PIN, RESET_PIN, -1, -1, LED_PIN, FLIP180, INVERT, BITBANG, SPEED);
    obdSetBackBuffer(&obd, ucBuffer);
    //obdSetRotation(&obd, 270);
    // Create some simple content
    obdFill(&obd, 0, 0);
    obdWriteString(&obd,0,0,0,"OneBitDisplay", FONT_16x32, OBD_BLACK, 0);
    for (int i=0; i<400; i+=3) {
        obdDrawLine(&obd, i, 0, 399-i, 239, OBD_BLACK, 0);
        obdDumpBuffer(&obd, NULL);
    }
   // for (int i=0; i<240; i+=3) {
   //     obdDrawLine(&obd, 399, i, 0, 239-i, OBD_BLACK, 0);
   // }
    obdDumpBuffer(&obd, NULL);
    while (!iStop) { // toggle the EXTCOM signal until the user hits CTRL-C
	    AIOWriteGPIO(EXTCOM_PIN, LOW);
	    usleep(500000); // toggle ext com at 1hz
	    AIOWriteGPIO(EXTCOM_PIN, HIGH);
	    usleep(500000);
    }
    AIOWriteGPIO(DISP_PIN, LOW); // turn off LCD
    AIOShutdown();
    return 0;
} /* main() */
