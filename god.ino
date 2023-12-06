// Eileen Kuang & Elaine Qian
// Nov 30, 2023
// God
// Automatic pet feeder!!!!!!!!

// Include the servo library:
#include "Servo.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

// Variable declarations
int currentHour; // user input
int currentMin; // user input
int currentTime;
int countTime; // time the program is counting
int target;
int numFeeding; // how many times pet needs to be fed in a day
int targetTimes[0]; // array of feeding times

// Create a new servo object:
Servo myservo;

// Define the servo pin:
#define servoPin 9;

//keypad
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

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM ); // initializing keypad obj

//ultrasonic sensor
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
  delay(1000);

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

// prompt user input to set schedule
void getTimes(){
  lcd.print("How many times does your pet need to be fed in a day?");
  delay(1000);
  /*
  key pad user input stuff to get numFeeding
  */
  for(int i=0; i < numFeeding; i++){

    lcd.print("Enter meal time #" + i);
    delay(3000);
    // convert meal time to minutes
    /*
    key pad user input stuff to get target
    */
    targetTimes[i] = target;
  }
  lcd.print("Schedule confirmed!");
  delay(300);
}

// count time and return if it's a feeding time or not
bool time(){

  currentTime = (currentHour * 60) + currentMin;
  countTime = currentTime;

  while(true){

    if(countTime = 1440 - currentTime) { // check when to reset time
      countTime = 0; 
      currentTime = 0;
    }
    
    delay(60000); // count 1 minute
    countTime++;
    for(int i = 0; i < numFeeding; i++) {

      if(countTime == targetTimes[i]) 
        return true;

    }
    return false;
  }
}