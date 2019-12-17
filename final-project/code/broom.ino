
// arduino code for the broom robot

// this robot works using Radio Control. Radio waves are used to determine which pin is activated from  the remote control
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(3);


const int threshold = 700;

void setup() {
  Serial.begin(9600);
  AFMS.begin();  // create with the default frequency 1.6KHz

  //initializing motors for wheels of the broom
  myMotor->setSpeed(100); //setting initial speed of left motor
  myOtherMotor->setSpeed(100); //setting initial speed of right motor
  myMotor->run(FORWARD); //testing left motor working
  myOtherMotor->run(FORWARD); //testing right motor working
  myMotor->run(RELEASE); //turning off motor in preparation for commands
  myOtherMotor->run(RELEASE); //turning of motor in preparation for  commands

}



void loop() {

  int forward = analogRead(A0); //assigning value coming from pin A0 to variable forward
  delay(1);        // delay between reads to allow cap to charge
  int reverse = analogRead(A1); //assigning value coming from pin A1 to variable reverse
  delay(1);        // delay between reads to allow cap to charge
  int right = analogRead(A3); //assigning value coming from pin A3 to variable right
  delay(1);        // delay between reads to allow cap to charge
  int left = analogRead(A2); //assigning value coming from pin A2 to variable left
  delay(1);        // delay between reads to allow cap to charge

  /* for debugging and to see what threshold to use
   
   Serial.print(right);
   Serial.print("\t");
   Serial.print(left);
   Serial.print("\t");
   Serial.print(reverse);
   Serial.print("\t");
   Serial.print(forward);
   Serial.println();
  
  */
  
  
  //this is the standard codes used however depending on which direction the wheels were put in, the forward and backward commands changed
  //this was tested before running performance and calibrated accordingly

  if (right > 700) { //700 is the value given when button pressed - equivelent to HIGH
    Serial.println("right");
    // send appropriate signals to motor
    // shield to turn right
    myMotor->run(BACKWARD); //makes the left motor turn backward
    myOtherMotor->run(FORWARD); //makes the right motor turn forward

  }
  if (left > 700) { //used as a stoping mechanism
    Serial.println("left");
    // shield to stop
    myMotor->run(RELEASE); //makes the left motor stop moving
    myOtherMotor->run(RELEASE); //makes the right motor stop moving
  }
  if (forward > 700) {
    Serial.println("forward");
    // shield to go forward
    myMotor->run(FORWARD); //makes the left motor turn backward
    myOtherMotor->run(FORWARD); //makes the right motor turn backwards
  }
  if (reverse > 700) {
    Serial.println("reverse");
    // shield to go backward
    myMotor->run(BACKWARD); //makes the left motor turn forward
    myOtherMotor->run(BACKWARD); //makes the right motor turn forward
  }
}
