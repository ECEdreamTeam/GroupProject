
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 9, 5, 4, 3, 8);

int pin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);  
}

void loop() {
  
  Serial.println(digitalRead(pin));
  
  if (digitalRead(pin)==1 ){
    lcd.clear();
    lcd.print("Door Open");
  }
 
  else if (digitalRead(pin) == 0) {
    lcd.clear();
    lcd.print("Door Closed");
  }
}
