#include <Servo.h>

Servo servo;

const int MIN_ANGLE = 30;
const int MAX_ANGLE = 180;

int buttonPin = 12;
boolean drinking; // states whether the system is switched to drinkng/not drinking

void setup() {
  Serial.begin(9600);
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo.write(MIN_ANGLE);
  pinMode(buttonPin, INPUT);
  drinking = false; // init drinking to false
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  boolean prevState = drinking;
  
  if(buttonState == HIGH)
    // switch drinking state
    drinking = !drinking;

  Serial.println(drinking);

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

void moveSlow(int from, int to, int step) {
    for(int pos = from; pos <= to; pos += step) { // goes from 0 degrees to angle
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
  }
}
