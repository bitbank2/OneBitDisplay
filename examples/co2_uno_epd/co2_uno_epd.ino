//
// OneBitDisplay EPD (e-ink) demo sketch
// Written by Larry Bank 10/10/2022
//
// This program shows how to use epd panels
// with the OneBitDisplay library. The general idea
// is that it doesn't support "immediate" mode with e-ink. 
// You'll need to allocate a buffer to hold the pixel
// data, draw on it, then dump it all at once to the panel
// as a full update (display) or partial (displayPartial).
// If your MCU doesn't have enough RAM to hold the framebuffer
// then you can use the 'zero buffer' mode as shown below.
// The orientation and drawing functions are more limited,
// but an MCU with constrained resources can still display
// attractive content.
//
#include <OneBitDisplay.h>
#include <SCD41.h>
#include "Roboto_Black_28.h"

ONE_BIT_DISPLAY lcd;
SCD41 mySensor;

// Define both of these to be -1 for default I2C pins
#define SDA_PIN -1
#define SCL_PIN -1
#define BITBANG false

#if defined(ARDUINO_AVR_UNO)
#define EPD_FREQ 2000000
#define EPD_CS 10
#define EPD_DC 9
#define EPD_BUSY 7
#define EPD_RES 8
#define EPD_MOSI -1
#define EPD_SCK -1
#define POWER_ENABLE -1
#define EPD_TYPE EPD213_122x250
#endif
void setup()
{
  lcd.setSPIPins(EPD_CS, EPD_MOSI, EPD_SCK, EPD_DC, EPD_RES, EPD_BUSY);
  lcd.SPIbegin(EPD_TYPE, EPD_FREQ);
  lcd.setRotation(90);
  if (mySensor.init(SDA_PIN, SCL_PIN, BITBANG, 100000) == SCD41_SUCCESS)
  {
    mySensor.start(); // start sampling mode
  } else { // can't find the sensor, stop
    lcd.fillScreen(OBD_WHITE);
    lcd.println("SCD41 not found!");
    lcd.println("Halted...");
    lcd.display();
    while (1) {};
  }
} /* setup() */

void loop() {

  mySensor.getSample();
  lcd.fillScreen(OBD_WHITE); // 0 for EPDs is white (for OneBitDisplay)
  lcd.setTextColor(OBD_BLACK);
  lcd.setFont(FONT_12x16);
  lcd.println("2.13 250x122 AVR test");
  lcd.setFreeFont(&Roboto_Black_28);
  lcd.setCursor(0,48);
  // Display CO2 value
  lcd.print("CO2 ");
  lcd.print(mySensor.co2(), DEC);
  lcd.println("ppm");

  // Display temperature
  lcd.print("Temperature: ");
  lcd.print(mySensor.temperature(), DEC);
  lcd.println("C");
  
  // Display humidity
  lcd.println("Humidity: ");
  lcd.print(mySensor.humidity(), DEC);
  lcd.println("%");

  // Tell EPD to do a full refresh
  lcd.display();
  delay(30000); // get a new sample every 30 seconds
}
