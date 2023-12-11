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
int targetTimes[10]; // array of feeding times
String input = "";
char key;
int b = 0;
int a = 180;
int flag = 0;

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

// lcd display
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// piezo!
const int PIE = 10;

void setup() {

  Serial.begin(9600);
  delay(1000);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIE, OUTPUT);

  lcd.begin(16, 2); //sets up rows and columns on the screen
  lcd.clear();

  myservo.attach(8);
  myservo.write(60);

  lcd.print("    Welcome:");
  lcd.setCursor(0, 1);
  lcd.print("Auto Pet Feeder!");
  delay(5000);

  getTimes();

  getCurrentTime();

  currentTime = (currentHour * 60) + currentMin;
  countTime = currentTime;
}

void loop() {

  if(time()){
    //releasing food 
    //add if statement for feeding times
    myservo.write(60);

    delay(500);

    myservo.write(0); // turn left

    delay(400);

    myservo.write(60);
  } 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Next feeding time: ");
  lcd.setCursor(0,1);
  lcd.print("");

  if(getDistance()>15.00 && flag%2 == 0){
    tone(PIE, 1047); // C6
    delay(200);
    tone(PIE, 1319); // E5
    delay(200);
    tone(PIE, 1047); // C6
    delay(200);
    tone(PIE, 988); // B5
    delay(200);
    tone(PIE, 1047); // C6
    delay(200);
    noTone(PIE);
  }
  flag++;

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

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("How many times");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print("in a day?");

  char key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  numFeeding = key - '0';

  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print(numFeeding);
  delay(3000);
  lcd.clear();

  for(int i=0; i < numFeeding; i++){

    lcd.setCursor(0,0);
    lcd.print("Enter hr for");
    lcd.setCursor(0, 1);
    lcd.print("meal time #");
    lcd.print(i+1);
    delay(800);

    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print(key);
    target = 10*(key - '0');
    
    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    lcd.print(key);
    target = target + (key - '0');
    delay(1000);
    lcd.clear();

    lcd.print(target);

    delay(1000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Enter min for");
    lcd.setCursor(0,1);
    lcd.print("meal time #");
    lcd.print(i+1);
    delay(800);

    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print(key);
    target = 60*target + 10*(key-'0');
    
    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    lcd.print(key);
    target = target + (key - '0');
    delay(1000);
    
    targetTimes[i] = target;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Schedule");
  lcd.setCursor(0, 1);
  lcd.print("  confirmed!");
  delay(3000);
  lcd.clear();
}

void getCurrentTime(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("What is the");
  lcd.setCursor(0,1);
  lcd.print("current hour?");
  
  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(key);
  currentHour = 10*(key-'0');

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  lcd.print(key);
  currentHour = currentHour + (key-'0');
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("What is the");
  lcd.setCursor(0,1);
  lcd.print("current minutes?");

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(key);
  currentMin = 10*(key-'0');

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  lcd.print(key);
  currentMin = currentMin + (key-'0');
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time confirmed!");
  delay(3000);
  lcd.clear();
}

// count time and return if it's a feeding time or not
bool time(){

  if(countTime == currentTime + 1440) {
    countTime = 0;
  }

  delay(60000); // count 1 minute
  countTime++;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(countTime);

  for(int i = 0; i < numFeeding; i++) {

    if(countTime == targetTimes[i]) 
      return true;

  }
  
  return false;

}

