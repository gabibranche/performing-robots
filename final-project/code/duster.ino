// code to make the duster work


// the duster works by using a dc motor which spins in one direction then the other
// it is controlled using a remote control triggering an IR sensor


// constants won't change. They're used here to set pin numbers:
const int buttonPin = A1;     // the number of the pushbutton pin

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

  // initialize the pushbutton pin as an input:
  // this pin will determin whether the remote has been pressed using an IR Sensor
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);           // set up Serial library at 9600 bps and is used for debugging
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(100);
   
}


void loop() {
  // read the state of the IR Sensor value:
  buttonState = analogRead(buttonPin);
  //print the value read by the IR Sensor
  Serial.println(buttonState);

  // check if the pushbutton is pressed. If it is, the buttonState is more than 500:
  if (buttonState > 500) {
    Serial.print("Button Pressed");
     int i = 0;

  // start the motor in one direction and increase then decrease speed gradually
    myMotor->run(FORWARD);
    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      delay(10);
    }
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      delay(10);
    }

  // start speed in opposite direction and increase then decrease speed gradually
    myMotor->run(BACKWARD);
    for (i = 0; i < 255; i++) {
      myMotor->setSpeed(i);
      delay(10);
    }
    for (i = 255; i != 0; i--) {
      myMotor->setSpeed(i);
      delay(10);
    }

    //stop motor for 1 second before repeating process
    myMotor->run(RELEASE);
    delay(1000);                     // waits 15ms for the servo to reach the position
  }
  else {
    // if remote off botton pressed, IR sensor will read less than 500 and then motors will be turned off
    myMotor->run(RELEASE);
  }
}
