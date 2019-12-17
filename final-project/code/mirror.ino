/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/
#include <Servo.h>

Servo myservo;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = A0;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int pos = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  myservo.attach(11);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = analogRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState > 550) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    for (pos = 50; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 100; pos >= 50; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
