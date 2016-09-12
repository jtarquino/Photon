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
String messageForScreen = "Loading data....";

// create a software timer to get new data every minute
Timer refreshTimer(refreshTimems, getStatusData);

Led led(D0, D1, D2, false);
Color red(255, 0, 0);
Color blue(0, 0, 255);
Color green(0, 255, 0);
Color black(0, 0, 0);
Color currentColor(0, 0, 0);
Color white(255,255,255);

void setup()
{
  led.off();
  currentColor = white;
  initializeScreen();
  serialSetupForDebugging();
  //debugBreakPoint();
  // start the data retrieval timer
  refreshTimer.start();
  //subscribe to the response(callback) of the webhook
  Particle.subscribe("hook-response/get_LedData/", getStatusDataCallBack, MY_DEVICES);
  // Exposes the web api to change the color by demand
  Particle.function("setRGBColor", setRGBColor);
  //Expose api to force a refresh
  Particle.function("refresh", triggerRefresh);
}

void initializeScreen(){
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(7);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false); // turn off text wrapping so we can do scrolling
  x    = display.width(); // set scrolling frame to display width
  minX = -1500; // 630 = 6 pixels/character * text size 7 * 15 characters * 2x slower
}
int triggerRefresh(String parameter){
  getStatusData();
  return 1;
}
// Enables the serial port to debug connected to USB in the serial port
void serialSetupForDebugging(){
  Serial.begin(9600);   // open serial over USB
  Serial.println("Serial Port Initialized!");
}

void loop() {
  // Fade the light between red and blue every second
  if (!isDataReceived){
    led.flash(currentColor);
    if(isFirstTimeInTheLoop)
    {
      getStatusData();
      isFirstTimeInTheLoop = false;
    }
  }
  led.fade(currentColor, blue, 500);
  refreshScreen();
}

void refreshScreen(){
  display.clearDisplay();
  display.setCursor(x/2, 7);
  display.print(messageForScreen);
  display.display();
  if(--x < minX) x = display.width()*2;
}

void getStatusData(){
  // Invoke the web hook
  Particle.publish("get_LedData");
}

String fullResponse = "";
bool isResponseCompleted;
void getStatusDataCallBack(String name, String data){
  isResponseCompleted = false;
  if(data.length()<512) // Chunks are up to 512 Bytes
    isResponseCompleted = true;

  if(data.length()==512 && data[511]=='}') //JSON Object is completed (buggy if is 512 and also have a closing bracket)
    isResponseCompleted = true;

  fullResponse += data;
  if (isResponseCompleted){
    Serial.println(fullResponse);
    isDataReceived = true;
    String color = getColorFromJSON(fullResponse);
    setLedColorFromString(color);
    fullResponse = "";
  }
}

String getColorFromJSON(String jsonData){
  // The JSON portion for the ledcolor is expected in  the following format {"LedColor":"red","Message":...
  String ledColorType = String("LedColor");
  int indexLedColor = jsonData.indexOf(ledColorType);
  indexLedColor +=  ledColorType.length();
  indexLedColor += 3; // counts for the closing quote the colon and the open quote ":"
  int indexLastQuote = jsonData.indexOf("\"",indexLedColor);
  String ledColor = jsonData.substring(indexLedColor, indexLastQuote);
  return ledColor;
}

int setRGBColor(String rgbColor){
  isDataReceived= false;
  if (rgbColor.length() != 11)
    return -1;

  int r = rgbColor.substring(0,2).toInt();
  int g = rgbColor.substring(4,6).toInt();
  int b = rgbColor.substring(7,9).toInt();
  currentColor = Color(r,g,b);

  return 1;
}

void setLedColorFromString(String color){
  if(color=="red")
  {
    currentColor = red;
  }
  else if(color=="green")
  {
    currentColor = green;
  }
  else if (color=="blue")
  {
    currentColor = blue;
  }
  else
  {
    currentColor = black;
  }
}

void debugBreakPoint(){
  Serial.begin(9600);
  while(!Serial.available()) Particle.process();

  Serial1.begin(9600);  // open serial over TX and RX pins

  Serial.println("Hello Computer");
  Serial1.println("Hello Serial 1");
}
