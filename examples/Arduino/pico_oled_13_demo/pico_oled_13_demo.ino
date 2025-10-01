//
// OneBitDisplay WaveShare Pico OLED 1.3 demo sketch
// Written by Larry Bank 7/12/2022
//
#include <OneBitDisplay.h>

ONE_BIT_DISPLAY lcd;

#define PICO_FREQ 10000000
#define PICO_CS 9
#define PICO_LED -1
#define PICO_RES 12
#define PICO_DC 8
#define PICO_MOSI 11
#define PICO_SCK 10

void setup() {
  // Initialize the library by telling it how the display is connected
  lcd.setSPIPins(PICO_CS, PICO_MOSI, PICO_SCK, PICO_DC, PICO_RES, PICO_LED);
  lcd.SPIbegin(OLED_64x128, PICO_FREQ);

  // Set the orientation to draw in the direction we want
  lcd.allocBuffer(); // allocate a back buffer and keep it internal to the class
  lcd.setRotation(90); // the panel is actually 64 wide by 128 tall, but oriented 90 right
  // To draw characters rotated 90/270 we need to allocate a back buffer
  lcd.fillScreen(OBD_WHITE); // 0 for EPDs is white (for OneBitDisplay)
  lcd.setFont(FONT_12x16); // use the internal stretched/smoothed font
  lcd.println("WaveShare");
  lcd.println("Pico-OLED");
  lcd.println("Demo");
  lcd.display(); // update
} /* setup() */

void loop() {
int i;
 for (i=0; i<2047; i++) {
  int x, y;
  x = rand() & 127;
  y = rand() & 63;
  lcd.drawPixel(x, y, 1);
  if ((i & 3) == 3)
     lcd.display();
 }
 lcd.setFont(FONT_6x8);
 lcd.setScroll(true);
 lcd.fillScreen(OBD_WHITE);
 i = 0;
 while (1) {
  lcd.println(i++, DEC);
  lcd.display();
 }
}
