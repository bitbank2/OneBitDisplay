#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <armbianio.h>
#define OUTPUT GPIO_OUT
#define INPUT GPIO_IN
#define INPUT_PULLUP GPIO_IN_PULLUP
#define HIGH 1
#define LOW 0
#include "../src/OneBitDisplay.h"
#include "../src/obd.inl"

