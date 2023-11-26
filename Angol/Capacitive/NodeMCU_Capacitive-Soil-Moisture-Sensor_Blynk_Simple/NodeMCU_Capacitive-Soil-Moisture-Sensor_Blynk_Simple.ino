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
  For this example you'll need ESP8266 libraries:

  
  And Blynk libraries:


  App project setup:
    Value Display or SuperChart widget attached to V1
    Value Display or SuperChart widget attached to V2
 *************************************************************/


/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


/* Costumize this lines to configure the program */
/* <<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>> */

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

const int airValue = 630;     // Replace this value with raw sensor value in air
const int waterValue = 275;   // Replace this value with raw sensor value in water

const long messureInterval = 1000L;   // Messure interval in milliseconds
                                      // 1000L    = 1 second
                                      // 10000L   = 10 seconds
                                      // 60000L   = 1 minute
                                      // 600000L  = 10 minutes
                                      // 3600000L = 1 hour
                                     

#define percentPin 1        // Number of Blynk virtual pin (V1), where the data sent in percent
#define sensorvaluePin 2    // Number of Blynk virtual pin (V2), where the raw data sent

/* <<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>><<¤>> */


#define sensorPin A0

int soilMoistureValue = 0;
int soilMoisturePercent = 0;

BlynkTimer timer;



// This function messures and sends moisture data to Blynk
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor() {
  
  //Messurment and data conversation
  int soilMoistureValue = analogRead(sensorPin);
    Serial.println("Soil moisture raw value: ");
    Serial.print(soilMoistureValue);

  int soilMoisturePercent = map(soilMoistureValue, airValue, waterValue, 0, 100);
    Serial.println("Soil moisture value in percent: ");
    Serial.print(soilMoisturePercent);

  // Send values to Blynk
  Blynk.virtualWrite(percentPin, soilMoisturePercent);
  Blynk.virtualWrite(sensorvaluePin, soilMoistureValue);
}



void setup() {
  
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Messure and send data every 'messureInterval' seconds
  timer.setInterval(messureInterval, sendSensor);
}



void loop() {
  Blynk.run();
  timer.run();
}
