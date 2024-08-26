#include <MKRWAN.h>
#include <stdio.h>
#include <sstream>

using namespace std;

LoRaModem modem;

#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {
  pinMode(A1, INPUT);

  Serial.begin(115200);

  while (!Serial);
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

  modem.minPollInterval(60);
}

#define WAITBEFOREAUTOSEND (120) // every 20 minutes

void loop() {
  int sensor = analogRead(A1);

  Serial.println(sensor);

  delay(2);

  int err;

  char sensor2[4];

  sprintf(sensor2, "%04d", sensor);

  Serial.println(sensor2);

  modem.beginPacket();
  modem.print(sensor2);
  err = modem.endPacket(true);

  if (err > 0) {
    Serial.println("Message sent correctly.");
  } else {
      Serial.println("Error sending message :(");
      Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
      Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }

  delay(120000);

  if (!modem.available()) {
  //  Serial.println("No downlink message received at this time.");
    return;
  }

  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  switch (rcv[0]) {
  case 0x00:
    Serial.print("led off");
    digitalWrite(LED_BUILTIN, 0);
    break;
  case 0x01:
    Serial.print("led on");
    digitalWrite(LED_BUILTIN, 1);
    break;
  default:
    Serial.print("unexpected payload");
    Serial.print(rcv[0] + 32);
    break;
  }
  Serial.println();
}
