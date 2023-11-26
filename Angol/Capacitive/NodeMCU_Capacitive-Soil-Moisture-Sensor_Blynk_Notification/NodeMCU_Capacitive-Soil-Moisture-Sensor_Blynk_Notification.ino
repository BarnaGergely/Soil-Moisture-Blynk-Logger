/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

  Blynk library is licensed under MIT license

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need ESP8266WiFi library.


  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define sensorPin A0


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";


int wateringLevel = 300;

const int DryValue = 650;   //you need to replace this value with value in air
const int WaterValue = 250;  //you need to replace this value with value in water

int soilMoistureValue = 0;
int soilMoisturePercent = 0;

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{


  //Messurment and data conversation
  soilMoistureValue = analogRead(sensorPin);
  soilMoisturePercent = map(soilMoistureValue, DryValue, WaterValue, 0, 100);


  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, soilMoistureValue);
  Blynk.virtualWrite(V6, soilMoisturePercent);
}

void notifyWatering()
{
  // Actually send the message.
  // Note:
  //   We allow 1 notification per 5 seconds for now.
  if (soilMoistureValue > wateringLevel)
  {
    Serial.print("Notification: ");
    Serial.println("Water your plant!");

    Blynk.notify("Water your plant!");
    // You can also use {DEVICE_NAME} placeholder for device name,
    // that will be replaced by your device name on the server side.
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

  // Setup a function to be called every minute
  timer.setInterval(60000L, notifyWatering);
}

void loop()
{
  Blynk.run();
  timer.run();
}
