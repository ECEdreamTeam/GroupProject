//This code is uploaded to the master unit of the JAAZ security system
//ECE387
//Group Project
/*
 * Jake Brumfield
 * Amanda Ryerse
 * Ashley Youngwirth
 * Zane Shreve
 */


//Import required libraries, including built-in SPI and Servo libraries
#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

//-----------------KEYPAD STUFF
#include <Keypad.h>
//Define the codes to lock/unlock the system
char* locks = "8675";
char* unlocks = "5399";

//Variables to keep track of the keypad's entered code
int position = 0;
int pos2 = 0;
boolean checker = false;

boolean isLocked = false;

//Setup up the keypad design (using only numbers 1-9)
const byte rows = 3;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2', '3'},
  {'4','5', '6'},
  {'7','8', '9'}
};
//Declare the pins for the keypad
byte rowPins[rows] = {0,8,7};
byte colPins[cols] = {4,3,2};

//Instantiate the keypad
Keypad keypad = Keypad(makeKeymap(keys),
                       rowPins, colPins,
                       rows, cols);



//--------------RF24 STUFF-------------------------

//Attachment to SPI bus is implicit:
//RF24 SCK to Pin 13
//RF24 MOSI to Pin 11
//RF24 MISO to Pin 12

//RF24 VCC to 3.3 V
//RF24 gnd to gnd

//Attach the RF24 module control pins 
RF24 radio(9, 10);


//Set addresses for communication pipes (same as transmitter)
const uint64_t pipe1 = 0xE8E8F0F0E1LL;
const uint64_t pipe2 = 0xE8E8F0F0E2LL;
const uint64_t pipe3 = 0xE8E8F0F0E3LL;

//Int to hold data values to pass along
int data;

//Boolean values for flags
bool caution = false;
bool threat = false;


void setup() {
  //Serial monitor for debugging purposes
  Serial.begin(9600);

  pinMode(6, OUTPUT);

  //Activate the RF24
  radio.begin();
  //Communicate on an open channel (same as transmitter)
  radio.setChannel(108);
  //Tell the RF234 we're going to be receiving on three pipes
  radio.openReadingPipe(1,pipe1);
  radio.openReadingPipe(2, pipe2);
  radio.openReadingPipe(3, pipe3);
  //Begin receiving
  radio.startListening();

  //Start the keypad as unlocked
  setLocked(false);
}

void loop() {    

  //See what has been typed into the keypad
  checkKeyPad();

  //If the keypad is locked (system is active)
  if (isLocked) {
    //Only enter this section of code if there is data to read                                 
    if (radio.available()) {                           
      //Receive the data from the transmitter
      radio.read(&data, sizeof(data));

      //Print the received data (for debugging)
      Serial.println(data);


      //Check the data that has been read in
      switch (data) {
        //ultrasonic #1 is not detecting
        case 0:
          caution = false;
          threat = false;
          break;
        //ultrasonic #1 is detecting
        case 1:
          caution = true;
          threat = false;
          break;
        //ultrasonic #2 is not detecting
        case 2:
          caution = false;
          threat = false;
          break;
        //ultrasonic #2 is detecting  
        case 3:
          caution = true;
          threat = false;
          break;
        //PIR is not detecting
        case 4:
          threat = false;
          break;
        //PIR is detecting
        case 5:
          //Check the caution flag
          if (caution){
            threat = true;
          }
          break;
      }
    }

    //Sound the alarm is the threat flag is high
    if (threat){
      tone(5, 3000, 2000);
    }
    
  }
}


//Method to check keypad input
void checkKeyPad() {
  char key = keypad.getKey();
  Serial.print(key);
  

  //Keeps track of the latest entered key
  if (key == locks[position])
    position++;

  //Advance the keypad if the next entered key is correct
  if (key == unlocks[pos2])
    pos2++;

 //If lock code has been entered correctly, lock the system
  if (position == 4) {
    checker = true;
    setLocked(true);
    position = 0;
  }

  //If unlock code has been entered correctly, unlock the system
  if (pos2 == 4){
    setLocked(false);
    checker = false;
    pos2 = 0;
  }
  
  delay(50);
}

//Locks the system
void setLocked(int locked){
  //if true, system is locked
  if (locked) {
    Serial.println("LOCKED!");
    isLocked = true;
  }
  //otherwise, system is unlocked
  else {
    Serial.println("UNLOCKED!");
    isLocked = false;
  }
}
