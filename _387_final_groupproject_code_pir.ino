///#include <elapsedMillis.h> // timer library
#include <SPI.h> // needed in Arduino 0019 or later
#include "RF24.h"

//Attachment to SPI bus is implicit:
//RF24 SCK to Pin 13
//RF24 MOSI to Pin 11
//RF24 MISO to Pin 12

//RF24 VCC to 3.3 V
//RF24 gnd to gnd

//Attach the RF24 module control pins to 9 & 10
RF24 radio(7,8);


//Set an address for a communication pipe (same as receiver)
//const uint64_t pipe1 = 0xE8E8F0F0E1LL;
const uint64_t pipe2 = 0xE8E8F0F0E2LL;

int data;                     //Int array to hold data values to pass along
int ledPin = 4;                // choose the pin for the LED
int inputPin = A0;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
//const int buzzer = 2;           // buzzer to arduino digital pin 2
int counter = 0;                // counter starts at zero
int motiontime = 0;             // time of motion
int flag = 0;                   // this keeps track of the time and whether motion is detected or not

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 // pinMode(buzzer, OUTPUT); // Set buzzer - pin 2 as an output
  Serial.begin(9600);

  //Activate the RF24
  radio.begin();
  //Communicate on an open channel (same as receiver)
   radio.setChannel(108); 
  //Tell the RF24 we're going to be transmitting
  //Open pipe2 for Amanda's arduino
  radio.openWritingPipe(pipe2);

}

void pir(){
  val = analogRead(inputPin);  // read input value
  //Serial.println(val);
  if (val > 200) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON

    if(flag == 0)
    {
      counter = 0;
    }
    
    flag = 1;

// option to implement a timer
//alarm only sounds if the motion has been detected for 30 seconds or more
  if (counter >= 30 && flag ==1 ){ 
 
//    tone(buzzer, 1000); // Send 1KHz sound signal...
//    delay(100);        // ...for 1 sec
//    noTone(buzzer);     // Stop sound...
//    delay(100);        // ...for 1sec

}

    if (pirState == LOW) 
    {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      data = 5; //motion output
    }
  } 
  else
  {
    digitalWrite(ledPin, LOW); // turn LED OFF

    if(flag == 1)
    {
        motiontime = counter - 8;
    }

    flag = 0;
    
      if(pirState == HIGH)
      {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
      data = 4; //no motion output
    }
  }
delay(800);
  counter = counter + 1;
  Serial.println(counter);
}

void loop () {
  pir(); // runs the PIR function
  radio.write(&data, sizeof(data)); //sends data to master unit
  
}

