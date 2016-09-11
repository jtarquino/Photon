#include <rgb-controls.h>
using namespace RGBControls;


Led led(D0, D1, D2, false);
Color red(255, 0, 0);
Color blue(0, 0, 255);
Color green(0, 255, 0);
Color black(0, 0, 0);
Color currentColor(0, 0, 0);

void setup()
{
  led.setColor(blue);
  delay(1000);
  led.setColor(red);
  delay(1000);
  led.setColor(green);
  delay(1000);
  led.off();
  delay(1000);
}

void loop() {
  // Fade the light between red and blue every 5 seconds
  led.fade(red, blue, 5000);
}
