#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);

const uint64_t pipe1 = 0xE8E8F0F0E1LL;
const int trigPin = 4;
const int echoPin = 5;

int data;
int doorPin = 3;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(108); 
  radio.openWritingPipe(pipe1); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(doorPin, OUTPUT);
}

void loop() {
  long duration, inches, cm;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);

  if (inches < 56 && data == 1){
     data = 1;
     
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(data);
  Serial.print(" data");
  Serial.println();
  digitalWrite(doorPin, HIGH);

  delay(100);

  }
   
  else if (inches < 56 && data !=1){
    data = 1;
    
  radio.write(&data, sizeof(data)); 
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(data);
  Serial.print(" data");
  Serial.println();
  digitalWrite(doorPin, HIGH);

  delay(100);

  }
  else if (inches >= 56 && data !=0){
    data = 0;
    
  radio.write(&data, sizeof(data));
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(data);
  Serial.print(" data");
  Serial.println();
  digitalWrite(doorPin, LOW);

  delay(100);

  }
  else{
    data = 0;
   
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(data);
  Serial.print(" data");
  Serial.println();
  digitalWrite(doorPin, LOW);

  delay(100);
  }
  
}

 long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}



