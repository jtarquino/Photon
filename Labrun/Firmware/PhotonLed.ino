
int led1 = D4;
int led2 = D7;

void setup()
{

   // Here's the pin configuration, same as last time
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);

   // We are also going to declare a Spark.function so that we can turn the LED on and off from the cloud.
   Particle.function("led",ledToggle);
  // This is saying that when we ask the cloud for the function "led", it will employ the function ledToggle() from this app.

   // For good measure, let's also make sure both LEDs are off when we start:
   digitalWrite(led1, LOW);
   digitalWrite(led2, LOW);

}

void loop()
{
   // Nothing to do here
   // run every 5 minutes call web service and set the color and the message
   // also if the webservice says do nothing turn off the led and screen
}

int setLedColor(String color){
  if(color=="red")
  {
    return 1;
  }
  else if(color=="green")
  {
    return 1;
  }
  else if (color=="blue")
  {
    return 1;
  }
  else
  {
    return 0;
  }

}

int setScreenMessage(String message){

}

int ledToggle(String command) {


    if (command=="on") {
        digitalWrite(led1,HIGH);
        digitalWrite(led2,HIGH);
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        return 0;
    }
    else {
        return -1;
    }
}
