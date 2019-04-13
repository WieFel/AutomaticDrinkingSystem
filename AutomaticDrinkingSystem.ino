// Adafruit IO Subscription Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <avr/dtostrf.h>
#include <Servo.h>

Servo servo;

const int MIN_ANGLE = 30;
const int MAX_ANGLE = 180;

int buttonPin = 12;
boolean prevState;
boolean drinking; // states whether the system is switched to drinkng/not drinking

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("motor-button");

void setup() {  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  counter->onMessage(handleMessage);

  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  counter->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // Do our setup for motor
  Serial.begin(9600);
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo.write(MIN_ANGLE);
  pinMode(buttonPin, INPUT);
  drinking = false; // init drinking to false
  prevState = drinking;

}

void loop() {
  // it should always be present at the top of loop
  io.run();

  int buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH) {
    // switch drinking state
    Serial.print("Button pressed: ");
    prevState = drinking;
    drinking = !drinking;
    Serial.println(drinking);
  }

  if(prevState != drinking) {
    // state changed -> rotate
    if(drinking) {
      //moveSlow(MIN_ANGLE, MAX_ANGLE, 1);
      servo.write(MAX_ANGLE);
        delay(200);
    }
    else {
      //moveSlow(MAX_ANGLE, MIN_ANGLE, -1);
      servo.write(MIN_ANGLE);
        delay(200);
    }
  }

}


void handleMessage(AdafruitIO_Data *data) {
  boolean val = data->value();
  if(val) {
    Serial.print("received <- ");
    Serial.println(val);
    prevState = drinking;
    drinking = !drinking;
  }
}
