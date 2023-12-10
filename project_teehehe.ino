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
String input = "";
char key;
int b = 0;
int a = 180;
bool flag = true;
bool galf = false;

// Create a new servo object:
Servo myservo;

// Define the servo pin:
// #define servoPin;

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

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.begin(16, 2); //sets up rows and columns on the screen
  lcd.clear();

  myservo.attach(8);

  myservo.write(60);

  delay(500);

  myservo.write(0);

  delay(400);

  myservo.write(60);

}

void loop() {
  
  myservo.attach(8);

  // myservo.write(0);

  // delay(100);

  // myservo.write(180);

  // delay(100);
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

  lcd.print("How many times does your pet");
  delay(5000);
  lcd.print("need to be fed in a day?");

  while (Serial.available() == 0) {
  }

  numFeeding = Serial.parseInt();

  Serial.println(numFeeding);

  for(int i=1; i <= numFeeding; i++){

    lcd.print("Enter hour for meal time #" + i);
    delay(100);


  while (Serial.available() == 0) {
  }

    target = 60*Serial.parseInt();

    lcd.print("Enter minutes for meal time #" + i);
    delay(100);

  while (Serial.available() == 0) {
  }

    target = target + Serial.parseInt();
    // convert meal time to minutes

    targetTimes[i] = target;
    
  }
  lcd.print("Schedule confirmed!");
  delay(300);
}

void getCurrentTime(){
  lcd.print("What is the current hour?");
  delay(100);

  while (Serial.available() == 0) {
  }

  currentHour = Serial.parseInt();

  lcd.print("What is the current minutes?");
  delay(100);

  while (Serial.available() == 0) {
  }

  currentMin = Serial.parseInt();

  lcd.print("Time confirmed!");
}

// count time and return if it's a feeding time or not
bool time(){

  currentTime = (currentHour * 60) + currentMin;
  countTime = currentTime;

  while(true){

    if(countTime == currentTime + 1440) {
      countTime = 0;
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

void releaseFood1(){

  for (int pos = 90; pos <= 180; pos ++) {
    myservo.write(pos);  // Set the servo position
    delay(5);           // Wait for the servo to reach the position
  }
  
}

void releaseFood2(){

  // Sweep back from 180 to 0 degrees
  for (int pos = 180; pos >= 90; pos --) {
    myservo.write(pos);  // Set the servo position
    delay(5);           // Wait for the servo to reach the position
  }

}