// Include the servo library:
#include "Servo.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

// Create a new servo object:
//Servo myservo;

// Define the servo pin:
//#define servoPin 9

//setup for keypad
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//code for ultrasonic sensor
const int TRIG_PIN = 20;
const int ECHO_PIN = 21;

long duration;
int distance;
int count = 0;

const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  // Attach the Servo variable to a pin:
  //myservo.attach(servoPin);

//sweep from 0 to 180
//  for (int angle = 0; angle <= 180; angle += 1) {
//    myservo.write(angle);
//    delay(15);
//  }

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.begin(16, 2); //sets up rows and columns on the screen

  Serial.print("hola");

  Serial.begin(9600);

  lcd.clear();

}

//back from 180 to 0
//  for (int angle = 180; angle >= 0; angle -= 1) {
//    myservo.write(angle);
//    delay(15);
//}

void loop() {
  char key = keypad.getKey();

  if(key) {
    Serial.println(key);

  if(count == 2) {   
      delay(500);    
      lcd.clear();
      count = 0;
    }
    lcd.print(key);  
    count++; 

    if (key == '#') {
      Serial.println(getDistance());
      lcd.print(getDistance());
    }

  }

}

//distance function for ultrasonic sensor
double getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delay(2);
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034/2.0;

    return distance;
}