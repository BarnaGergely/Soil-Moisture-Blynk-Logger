/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V1
    Value Display widget attached to V2
 *************************************************************/

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
BlynkTimer timerNotification;


/* <>|<>|<>|<>|<><><><><><><><><><><><><><><><><><><><><><><><><><><><>< */

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define sensorPin A0        //What analog pin where the sensor connected
#define sensorPower D1     // What digital pin where the sensor get the power

/* <>|<>|<>|<>|<><><><><><><><><><><><><><><><><><><><><><><><><><><><>< */


// This function sends Arduino's up time every second to Virtual Pin.
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor() {

  // Turn on moisture sensor.
  digitalWrite(sensorPower, HIGH);
  delay(500); // wait for 0.5 second to prepare the sensor. Further testing required.

  // Messurment and data conversation
  int sensorValue = analogRead(sensorPin);
  int outputValue = map(sensorValue, 0, 1023, 100, 0);

  delay(100);
  digitalWrite(sensorPower, LOW); //Turn off sensor

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, outputValue);
  Blynk.virtualWrite(V2, sensorValue);
}

void setup() {

  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  pinMode(sensorPower, OUTPUT); // set the digital pin as output to power the sensor
  digitalWrite(sensorPower, LOW);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop() {

  Blynk.run();
  timer.run();

}
