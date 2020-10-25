//
// Small Simple OLED library multi-display demo
//
// Demonstrates how to initialize and use multiple displays
//
#include <OneBitDisplay.h>

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are reversed because I did straight-through wiring for my SSD1306
// and it has the 4-pin header as GND,VCC,SCL,SDA, but the GROVE connector is
// GND,VCC,SDA,SCL
#define GROVE_SDA_PIN 32
#define GROVE_SCL_PIN 26
// Heltek LoRa Wifi 868Mhz w/OLED
//#define GROVE_SDA_PIN 4
//#define GROVE_SCL_PIN 15

// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN 16
//#define RESET_PIN -1
// let OneBitDisplay figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 1
#define BUTTON_ACTIVE LOW
// M5 Atom main button
#define ENTER_BUTTON 39
// Heltek ESP32 LoRa PROG button
//#define ENTER_BUTTON 13

SIMPLEMENU sm;

char *menu1[] = {(char *)"* Demo Menu *", (char *)"Menu Item 1", (char *)"Menu Item 2", (char *)"Menu Item 3",
(char *)"Menu Item 4", (char *)"Menu Item 5", (char *)"Menu Item 6", (char *)"Menu Item 7", (char *)"Menu Item 8",
(char *)"Menu Item 9", NULL};
// on/off state of each menu item
static uint8_t ucToggle[] = {0,1,0,0,1,1,1,0,0,0};
static char *szOnOff[] = {(char *)"Off", (char *)"On"};

// Change this if you're using different OLED displays
#define MY_OLED OLED_128x64
uint8_t ucBackBuffer[1024];

// The display structure. Each structure is about 56 bytes
// There is no limit to the number of simultaneous displays which can be controlled by OneBitDisplay 
OBDISP obd;

void setup() {
char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
int rc;
// The I2C SDA/SCL pins set to -1 means to use the default Wire library
// If pins were specified, they would be bit-banged in software
// This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
// The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
//    that can be configured as either SPI or I2C
//
// obdI2CInit(OBDISP *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)

// Enable power to OLED display on Heltec OLED board
//  pinMode(21, OUTPUT); // Vext
//  digitalWrite(21, LOW); // turn on

  rc = obdI2CInit(&obd, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, GROVE_SDA_PIN, GROVE_SCL_PIN, RESET_PIN, 800000L); // use standard I2C bus at 400Khz
  
  if (rc != OLED_NOT_FOUND)
  {
    obdFill(&obd, 0, 1);
    obdWriteString(&obd, 0,0,0,msgs[rc], FONT_8x8, 0, 1);
    delay(2000);
  }
  else
  {
    Serial.begin(115200);
    Serial.println("Error initializing OLED");
    while (1) {};
  }
  obdSetBackBuffer(&obd, ucBackBuffer);
} /* setup() */

char * myCallback(int iIndex)
{
  return szOnOff[ucToggle[iIndex]];
} /* myCallback() */

void loop() {
  int i;
  pinMode(ENTER_BUTTON, INPUT_PULLUP); // initialize the button inputs before calling the menu code
//int menuInit(OBDISP *pOBD, SIMPLEMENU *sm, char **pText, int iFontSize, int bCenter, int btnUp, int btnDn, int btnEnter, int iPressedState, int bIsRotary)
// Initialize the menu with a single button. In this mode, a quick press advances the position and a long press
// takes 'action' on the currently selected item
  obdMenuInit(&obd, &sm, menu1, FONT_8x8, 0, ENTER_BUTTON, -1, -1, BUTTON_ACTIVE, false);
  obdMenuSetCallback(&sm, myCallback);
  obdMenuShow(&sm, -1); // display the whole menu
  while (1) {
    i = obdMenuRun(&sm);
    if (i != -1) {
       ucToggle[i] = !ucToggle[i];
       obdMenuShow(&sm, i); // show new status of selected item
    }
    delay(15);
  }
} /* loop() */
