#include <rgb-controls.h>
using namespace RGBControls;

int const refreshTimems = 60000; //every minute
//int const refreshTimems = 10000; //every 10 seconds
bool isDataReceived = false;

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
  //serialSetupForDebugging();
    // start the data retrieval timer
  refreshTimer.start();
  //subscribe to the response(callback) of the webhook
  Particle.subscribe("hook-response/get_LedData/", getStatusDataCallBack, MY_DEVICES);
  // Exposes the web api to change the color by demand
  Particle.function("setRGBColor",setRGBColor);
  //initializes the first value of the led
  getStatusData();
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
  }
  led.fade(currentColor, blue, 1000);
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
    isDataReceived = true;
    String color = getColorFromJSON(fullResponse);
    setLedColorFromString(color);
    fullResponse = "";
  }
}


String getColorFromJSON(String jsonData){
  // The JSON portion for the ledcolor is expected in  the following format {"LedColor":"red","LabRunData":...
  Serial.println(jsonData);
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
