
#include <MKRWAN.h>
#include <CayenneLPP.h>
#include "src/propeller.h" 
#include "src/compass.h" 
#include "arduino_secrets.h"

LoRaModem modem;

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

CayenneLPP lpp(51);

void setup() {

  Serial.begin(115200);

  while (!Serial);
  Serial.println("start");

  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };

  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  Serial.println("connected");


  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.

  prop_setup();
  compass_setup();

}


void loop() {
 
  float windSpeed = getWindSpeed();
  float windDirection = getWindDirection();
  Serial.print(windSpeed);
  Serial.println(" Km/h");

  lpp.reset();
  lpp.addGenericSensor(1, windSpeed); //Wind speed in Km/h
  lpp.addDirection(2, windDirection); // Wind direction in degrees

  int err;
  modem.beginPacket();
  modem.write(lpp.getBuffer(), lpp.getSize());
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
    Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
    Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }

  delay(2000);

}

