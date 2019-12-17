#include <Servo.h>

Servo myservo;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = A1;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);


void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(100);
   
}


void loop() {
  // read the state of the pushbutton value:
  buttonState = analogRead(buttonPin);
  Serial.println(buttonState);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState > 500) {
    Serial.print("Button Pressed");
     int i = 0;

    myMotor->run(FORWARD);
    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      delay(10);
    }
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      delay(10);
    }


    myMotor->run(BACKWARD);
    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      delay(10);
    }
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      delay(10);
    }

    myMotor->run(RELEASE);
    delay(1000);                     // waits 15ms for the servo to reach the position
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    myMotor->run(RELEASE);
  }
}
