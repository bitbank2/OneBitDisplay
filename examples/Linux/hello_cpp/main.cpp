#include <OneBitDisplay.h>
ONE_BIT_DISPLAY lcd;
#define LCD_FREQ 1000000
#define LCD_CS 7
#define LCD_LED -1
#define LCD_RES 25
#define LCD_DC 9
// MOSI value is the SPI channel and SCK value is the CS (e.g. /dev/spidev<mosi>.<sck>)
// This defines /dev/spidev1.1 for Raspberry Pi
#define LCD_MOSI 0
#define LCD_SCK 1

int main(int argc, const char * argv[]) {
  // Initialize the library by telling it how the display is connected
//  lcd.setSPIPins(LCD_CS, LCD_MOSI, LCD_SCK, LCD_DC, LCD_RES, LCD_LED);
//  lcd.SPIbegin(LCD_ST7302, LCD_FREQ); // 250x122 LCD
  lcd.setI2CPins(1, 0); // SDA = I2C channel (/dev/i2c-1)
  lcd.I2Cbegin(OLED_128x64);
  lcd.allocBuffer(); // this LCD needs a back buffer
  lcd.fillScreen(OBD_WHITE);
  lcd.setFont(FONT_12x16);
  lcd.println("Hello World");
  lcd.display();
  return 0;
} /* main() */


