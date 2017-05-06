
// Ultra Sonic Sensor Code (PIPE 1 & PIPE 3)
// Names: Ashley Youngwirth, Zane Shreve, Amanda Ryerse, Jake Brumfield
// Project: JAAZ home security system
// Source Code: PIPE 3

#include <SPI.h>
#include "RF24.h"
#include <Servo.h> 

//Attachment to SPI bus is implicit:
//RF24 SCK to Pin 13
//RF24 MOSI to Pin 11
//RF24 MISO to Pin 12

//RF24 VCC to 3.3 V
//RF24 gnd to gnd`

//Attach the RF24 module control pins to 7 & 8
RF24 radio(7,8);

// Servo Motor variable
Servo motor; 
int state = 0;
int theta = 21;


// Set an address for a communication pipe (same as receiver)
const uint64_t pipe1 = 0xE8E8F0F0E1LL;
const uint64_t pipe2 = 0xE8E8F0F0E2LL;
const uint64_t pipe3 = 0xE8E8F0F0E3LL;

// UltraSonic pins
int trig = 4;
int echo = 3;

// value will be sent through RF transmitter
int data;

int detect = 0;

 long duration, inches;

void setup() {
  
  // initialize serial communication:
  Serial.begin(9600);

  radio.begin();

  motor.attach(10);
  //Communicate on an open channel (same as receiver)
  radio.setChannel(108);

   //Tell the RF24 we're going to be transmitting

  //Open pipe1 for Ashley's arduino
  //radio.openWritingPipe(pipe1); 
  //Open pipe2 for Amanda's arduino
  //radio.openWritingPipe(pipe2);

  //Open pipe3 for Jake's arduino
   radio.openWritingPipe(pipe3);

  //Open pipe4 for LCD screen
  //radio.openWritingPipe(pipe4);
}

void moveServo(){
  // Checks the position and changes states
    if (theta <= 20)
        state = 0;
    
    if (theta >= 160)
        state = 1;

 // Checks the state and moves the position of the servo motor
    if (state == 0)
        theta = theta+1;
    
    if (state == 1)
        theta = theta-1;

  // Moves the motor
  motor.write(theta);

}

void loop() {

  
  pinMode(trig, OUTPUT);              // 4 pin ultraSonic sensor
  //pinMode(ultra, OUTPUT);           // 3 pin ultraSonic sensor
  digitalWrite(trig, LOW);            // 4 pin ultraSonic sensor
  //digitalWrite(ultra, LOW);         // 3 pin ultraSonic sensor
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);           // 4 pin ultraSonic sensor
  //digitalWrite(ultra, HIGH);        // 3 pin ultraSonic sensor
  delayMicroseconds(5);
  digitalWrite(trig, LOW);            // 4 pin ultraSonic sensor
  //digitalWrite(ultra, LOW);         // 3 pin ultraSonic sensor

  pinMode(echo, INPUT);               // 4 pin ultraSonic sensor
  //pinMode(ultra, INPUT);            // 3 pin ultraSonic sensor
  
  duration = pulseIn(echo, HIGH);       // 4 pin ultraSonic sensor
  //duration = pulseIn(ultra, HIGH);   //3 pin ultraSonic sensor

  // Converts the duration of pings into inches
  inches = toInches(duration);

  // Moves the servo Motor
   moveServo();

    
  // Checks to see if if there is an object within 10 inches.
  
  // Object is still being picked up
    if (inches < 15 && detect < 7){
      data = 3;
      detect++;
     
    }
    
  // Object is detected for the first time
    else if (inches < 15  && detect >= 7){
      detect++;
      data = 3;
      radio.write(&data, sizeof(data));
    }
      
   // No objects are detected
    else if (inches >= 15 && detect < 7){
      data = 2;
      detect = 0; 
    }
      
  // Object is not detected for the first time since an object
  // has been detected or the alarm has been locked.
    else{
      data = 2;
      radio.write(&data, sizeof(data));
      detect = 0;

    }
  
  displaySerial();
  
  
}
// TESTING PURPOSES ONLY
void displaySerial(){
    
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(data);
    Serial.print(" data, ");
    Serial.print(theta);
    Serial.print(" angle, ");
    Serial.print(detect);
    Serial.print(" detect");
    Serial.println();
    delay(100);
}

// Method that converts the time of the pings to inches. 
long toInches(long microseconds) {return microseconds / 74 / 2;}

