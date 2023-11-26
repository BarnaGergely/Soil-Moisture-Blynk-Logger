/*********************************************************************

Ez a program megadott időközönként mér egy kapacitív 
talajnedvesség mérővel, feltölti az adatokat Blynk-re és értesítést
küld, ha túl száraz a talaj.

A projekt részletes bemutatóját itt találod:
      Még készül
      
**********************************************************************

Amire szükség lesz:
  - NodeMCU, Wemos D1 Mini vagy bármely ESP8266 alapú mikroszámítógép
  - Kapacitív talajnedvesség érzékelő
  - Jumper kábelek
  - Micro USB kábel és egy telefontöltő
  - WiFi kapcsolat, internet

Kapcsolás:
    3V    <----->   VCC
    GND   <----->   GND
    A0    <----->   AOUT

Szükséges könyvtárak:     
  - Blynk könyvtár:
        https://github.com/blynkkk/blynk-library/releases/latest
  - ESP8266 könyvtárai:
        https://github.com/esp8266/Arduino


Blynk alkalmazás beállításai:
  - Value Display vagy SuperChart Widget a V1-hez csatolva
  - Value Display vagy SuperChart Widget a V2-höz csatolva


Ha érdekelnek hasonló projektek, látogass el oldalamra.
    - Weboldal: https://minerktech.blog.hu/
    - Youtube: https://www.youtube.com/c/MinerkTech
    - Facebook: https://www.facebook.com/minerktech
    - GitHub: https://github.com/MinerkTech

Licensz: MIT License
*********************************************************************/

/* Tedd ezt megjegyzéssé, a prints letiltásához és hely felszabadításához*/
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

// This function sends Arduino's up time every second to Virtual Pins (5, 6).
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
  // Note: The server allow 1 notification per 5 seconds for now.
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
