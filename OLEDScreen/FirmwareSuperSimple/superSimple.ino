// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
String messageForScreen = "Loading data....";

int  x, minX; // variables for scrolling code

void setup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(7);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false); // turn off text wrapping so we can do scrolling
  x    = display.width(); // set scrolling frame to display width
  minX = -1500; // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}

void loop() {
  display.clearDisplay();
  display.setCursor(x/2, 7);
  display.print(messageForScreen);
  display.display();
  if(--x < minX) x = display.width()*2;
}
