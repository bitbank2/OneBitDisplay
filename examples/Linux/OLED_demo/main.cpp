//
//  main.cpp
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
#include <OneBitDisplay.h>

volatile int iStop = 0;
ONE_BIT_DISPLAY obd;

int main(int argc, const char *argv[])
{
	obd.setI2CPins(4,0x3c);
	obd.I2Cbegin(OLED_128x64);
	obd.fillScreen(OBD_WHITE);
	obd.setFont(FONT_12x16);
	obd.setTextColor(OBD_BLACK);
	obd.drawString("Hello",0,0);
	obd.drawString("World!",0,16);
    return 0;
} /* main() */
