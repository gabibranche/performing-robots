// code to make the self cleaning mirror work


// the duster works by using a servo motor and magnest which moves in one direction then the other
// it is controlled using a remote control triggering an IR sensor

//include library for operating servo
#include <Servo.h>
Servo myservo;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = A0;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int pos = 0;

void setup() {

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // this pin will determin whether the remote has been pressed using an IR Sensor
  myservo.attach(11);
}

void loop() {
  
  // read the state of the IR Sensor value:
  buttonState = analogRead(buttonPin);

  //print the value read by the IR Sensor
  Serial.println(buttonState);
  
   // check if the pushbutton is pressed. If it is, the buttonState is more than 500:
  if (buttonState > 550) {
    
    //creating a pendulum movement with servo 
    
    //moves servo in a arched position allowing for cloth to movie in that arch on 'mirror'
     for (pos = 50; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    
     //moves servo in a arched position in the opposite direction allowing for cloth to movie in that arch on 'mirror'
    for (pos = 100; pos >= 50; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else {
    // if remote off botton pressed, IR sensor will read less than 500 and then servo will be turned off
  }
}
