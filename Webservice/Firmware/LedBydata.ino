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

int triggerRefresh(String parameter){
  getStatusData();
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
  // Fade the light between red and blue every second
  if (!isDataReceived){
    led.flash(currentColor);
    if(isFirstTimeInTheLoop)
    {
      getStatusData();
      isFirstTimeInTheLoop = false;
    }
  }
  led.fade(currentColor, blue, 1000, 0);
  refreshScreen();
}

void refreshScreen(){
  display.clearDisplay();
  display.setCursor(x/2, fontSize);
  display.print(messageForScreen);
  display.display();
  if(--x < minX)
    x = display.width()*2;
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
    parseJsonAndSetValues(fullResponse);
    fullResponse = "";
  }
}

void parseJsonAndSetValues(String jsonString){
  char *json = &jsonString[0u];

  String color;
  String message;


  char *contents = strtok(json, "{}");//remove '{' and  '}' : note that is not included in the content
  char key[128], value[1000];
  int len;
  while(2==sscanf(contents, "\"%127[^\"]\":\"%999[^\"]\",%n", key, value, &len)){
    if(strcmp(key, "LedColor")==0)
      color = value;
    if(strcmp(key, "Message")==0)
        message = value;
    contents += len;
  }

  Serial.println("Color: " + color);
  Serial.println("Message: " + message);

  setLedColorFromString(color);
  minX = -1 * 6 * fontSize * message.length() * 2; //  6 pixels/character * text size  * characters * 2x slower
  messageForScreen = message;

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
