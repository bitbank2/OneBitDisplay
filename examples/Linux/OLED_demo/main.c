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
#include <pthread.h>

#include "../../src/OneBitDisplay.cpp"

volatile int iStop = 0;

#define OLED_TYPE OLED_128x64
#define OLED_ADDR 0x3c
#define FLIP180 0
#define INVERT 0
#define HW_I2C 1
// On Linux this is the I2C bus number (SDA_PIN) for HW I2C
#define SDA_PIN 1
// On Linux this is the I2C device address
#define SCL_PIN 0x3c
#define RESET_PIN -1
// This is not controlled on Linux through the API
// On the Raspberry PI, see /boot/config.txt
#define SPEED 0
#define MAX_THREADS 2
void *OLEDThread(void *);

OBDISP obd[MAX_THREADS];

void my_handler(int signal)
{
   iStop = 1;
} /* my_handler() */

int main(int argc, const char *argv[])
{
int i, ID[MAX_THREADS];
pthread_t tinfo[MAX_THREADS];
struct sigaction sigIntHandler;
size_t ret;

// Set CTRL-C signal handler
  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  for (i=0; i<MAX_THREADS; i++) {
     ID[i] = i; // thread number
     pthread_create(&tinfo[i], NULL, OLEDThread, &ID[i]);
  } // for i
  // wait for all of the threads to finish
  for (i=0; i<MAX_THREADS; i++) {
      pthread_join(tinfo[i], (void **)&ret);
      printf("Thread %d finished\n", i);
  }
} /* main() */

void *OLEDThread(void *pStruct)
{
//int width, height;
int *pID = (int *)pStruct;
uint8_t ucBuffer[1024];

   // width = 128; // assume SSD1306 128x64
   // height = 64;
    obdI2CInit(&obd[*pID], OLED_TYPE, -1, FLIP180, INVERT, HW_I2C, (*pID == 0) ? 1:3, (*pID == 0) ? 0x3c : 0x3d, RESET_PIN, SPEED);
    obdSetBackBuffer(&obd[*pID], ucBuffer);
    // Create some simple content
    obdFill(&obd[*pID], 0, 1);
    obdWriteString(&obd[*pID],0,0,0,"OneBitDisplay", FONT_8x8, 0, 1);
    while (!iStop) {
	obdFill(&obd[*pID], 0, 1);
	obdFill(&obd[*pID], 0xff, 1);
//       for (int i=0; i<128; i++) {
//          obdDrawLine(&obd, i, 0, 127-i, 63, 1, 1);
//       }
//       for (int i=0; i<64; i++) {
//          obdDrawLine(&obd, 127, i, 0, 63-i, 1, 1);
//       }
    }
//    obdEllipse(&obd, 320, 240+64, 150,100, 0, 1);
//    obdRectangle(&obd, 300, 240+32, 340, 240+96, 1, 1);
    return NULL;
} /* OLEDThread() */
