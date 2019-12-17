//arduino code for the bucket robot

//this robot is manipulated using adafruit bluetooth LE Friend. The commands to move the mop are sent using bluetooth and then determine which motors are turned on and off according to make the bucket move forward, backwards, left and right accordingly.

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

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

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines

// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];


void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  AFMS.begin();
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
    delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));

//initializing motors for wheels of the bucket
  myMotor->setSpeed(150); //setting initial speed of left motor
  myOtherMotor->setSpeed(150); //setting initial speed of right motor
  myMotor->run(FORWARD); //testing left motor working
  myOtherMotor->run(FORWARD); //testing right motor working
  myMotor->run(RELEASE); //turning off motor in preparation for commands
  myOtherMotor->run(RELEASE); //turning of motor in preparation for  commands

}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  /* Got a packet! */
  // printHex(packetbuffer, len);

  // Color
  if (packetbuffer[1] == 'C') {
    uint8_t red = packetbuffer[2];
    uint8_t green = packetbuffer[3];
    uint8_t blue = packetbuffer[4];
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);
  }

  // Buttons
  //this code was manipulated for the purpose of this machine
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0'; //initializing putton numbers 1-8
    boolean pressed = packetbuffer[3] - '0'; // initializing boolean for whether pressed or not

    //confirming which button pressed
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }


    
    //this is the standard codes used however depending on which direction the wheels were put in, the forward and backward commands changed
    //this was tested before running performance and calibrated accordingly

    //detecting if any buttons pressed
    if (pressed ) {
      
      //making mop move forward if button 5 (ie arrow up) pressed
      if (buttnum == 5) {
        Serial.print(" Forward");
        myMotor->run(FORWARD); //makes the left motor turn forward
        myOtherMotor->run(FORWARD); //makes the right motor turn forward
      }

      //making mop move backward if button 6 (ie arrow down) pressed
      if (buttnum == 6) {
        Serial.print(" Backward");
        myMotor->run(BACKWARD); //makes the left motor turn backward
        myOtherMotor->run(BACKWARD); //makes the right motor turn backwards
      }

      //making mop move spinning in an anticlockwise direction if button 7 (ie arrow left) pressed
      if (buttnum == 7) {
        myMotor->run(FORWARD); //makes the left motor turn forward
        myOtherMotor->run(BACKWARD); //makes the right motor turn backward
      }

      //making mop move spinning in an clockwise direction if button 8 (ie arrow right) pressed
      if (buttnum == 8) {
        myMotor->run(BACKWARD); //makes the left motor turn backward
        myOtherMotor->run(FORWARD); //makes the right motor turn forward
      }

      //making mop cease moving
      //although the buttons were only pressed once, the yellow plastic motors work on an command by command basis, meaning once turned on, they will keep going unless commanded other wise
      if (buttnum == 2) {
        myMotor->run(RELEASE); //makes the left motor stop moving
        myOtherMotor->run(RELEASE); //makes the right motor stop moving
      }
    }
  } // end of button event

