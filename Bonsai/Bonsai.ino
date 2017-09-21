#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <rgb-controls.h>
using namespace RGBControls;

// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

int  x, minX; // variables for scrolling code

int const refreshTimems = 60000; //every minute
//int const refreshTimems = 10000; //every 10 seconds
bool isDataReceived = false;
bool isFirstTimeInTheLoop = true;
int const fontSize = 4;
int const screenHeight = 64;
int const screenWidth = 128;
bool isGreenState = false;
String messageForScreen = "Jaime data....";

// create a software timer to get new data every minute
//Timer refreshTimer(refreshTimems, getStatusData);

Led _led(D0, D1, D2, false);
Color red(255, 0, 0);
Color blue(0, 0, 255);
Color green(0, 255, 0);
Color black(0, 0, 0);
Color currentColor(0, 0, 0);
Color white(255,255,255);
int _humidity;
int _humiditySensor = A0;
int _power = A1;

void setup()
{
  _led.off();
  currentColor = white;
  initializeScreen();
  serialSetupForDebugging();

  pinMode(_humiditySensor,INPUT);
  //pinMode(_power,OUTPUT); // not needed
  //digitalWrite(_power,HIGH);
  Particle.variable("humidity", &_humidity, INT);

  //debugBreakPoint();
  // start the data retrieval timer
  //refreshTimer.start();
  // Exposes the web api to change the color by demand
  Particle.function("setRGBColor", setRGBColor);
  //Expose api to force a refresh
  Particle.function("irrigate", irrigate);
  // Expose api to set the text
  Particle.function("setText", setText);
}

void initializeScreen(){
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(fontSize);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false); // turn off text wrapping so we can do scrolling
  x    = display.width(); // set scrolling frame to display width
  minX = -1 * 6 * fontSize * 15 * 2; // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}

int irrigate(String parameter){
  return 1;
}

int setText(String text){
  minX = -1 * 6 * fontSize * text.length() * 2;
  messageForScreen = text;
  return 1;
}
// Enables the serial port to debug connected to USB in the serial port
void serialSetupForDebugging(){
  Serial.begin(9600);   // open serial over USB
  Serial.println("Serial Port Initialized!");
}

void loop() {
  //_led.fade(currentColor, blue, 1000, 0);
  //_led.flash(currentColor);
  _led.setColor(currentColor);
  //currentColor = green;
  _humidity = analogRead(_humiditySensor);
  setText(String(_humidity));
  Particle.publish("bonsaiHumidity", String(_humidity), PRIVATE);
  refreshScreen();
  delay(10000); // get data every ten seconds
}

void getStatusData(){
  // Invoke the web hook
  //Particle.publish("get_LedData");
}

void refreshScreen(){
  display.clearDisplay();
  display.setCursor(x/4, fontSize);
  display.print(messageForScreen);
  display.display();
}

void refreshScreenScroll(){
  display.clearDisplay();
  display.setCursor(x/2, fontSize);
  display.print(messageForScreen);
  display.display();
  if(--x < minX)
    x = display.width()*2;
}


int setRGBColor(String rgbColor){
  isDataReceived = false;
  if (rgbColor.length() != 11)
    return -1;

  int r = rgbColor.substring(0,2).toInt();
  int g = rgbColor.substring(4,6).toInt();
  int b = rgbColor.substring(7,9).toInt();
  currentColor = Color(r,g,b);

  return 1;
}


void debugBreakPoint(){
  Serial.begin(9600);
  while(!Serial.available()) Particle.process();

  Serial1.begin(9600);  // open serial over TX and RX pins

  Serial.println("Hello Computer");
  Serial1.println("Hello Serial 1");
}
