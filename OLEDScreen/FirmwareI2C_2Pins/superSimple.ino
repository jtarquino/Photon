// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);
int  x, minX; // variables for scrolling code
int textSize = 2;

void setup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  display.setTextSize(textSize);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false); // turn off text wrapping so we can do scrolling
  x = display.width(); // set scrolling frame to display width
  minX = -1 * (6 *  textSize * 15 ); // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}

void loop() {
  display.clearDisplay();
  display.setCursor(x/2, textSize);
  display.print("Hello world");
  display.display();
  if(--x < minX) x = display.width()*2;
}
