// Eileen Kuang & Elaine Qian
// December 12, 2023
// Automatic pet feeder
// Sets scheduled feeding times and dispenses pet food

// Include libraries for servo, lcd, and keypad
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

// Create a new servo object:
Servo myservo;

// keypad
const int ROW_NUM = 4; // four rows
const int COLUMN_NUM = 4; // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {A7, A6, A5, A4}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {A3, A2, A1, A0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM ); // initializing keypad object

//ultrasonic sensor
const int TRIG_PIN = 20;
const int ECHO_PIN = 21;

long duration;
int distance;

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

  lcd.begin(16, 2); // sets up rows and columns on the screen
  lcd.clear();

  myservo.attach(8);
  myservo.write(60);

  // Welcome message
  lcd.print("    Welcome:");
  lcd.setCursor(0, 1);
  lcd.print("Auto Pet Feeder!");
  delay(5000);

  getTimes();

  getCurrentTime();

  // converting time
  currentTime = (currentHour * 60) + currentMin;
  countTime = currentTime;
}

void loop() {

  if(time()){
    //releasing food 
    myservo.write(60);
    delay(500);
    myservo.write(0); // turn left
    delay(400);
    myservo.write(60);
  } 

  // piezo alert when food supply is low
  if(getDistance()>14.00){
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

  // prompts user input for number of feeding times per day
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

  // Confirmation message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Schedule");
  lcd.setCursor(0, 1);
  lcd.print("  confirmed!");
  delay(3000);
  lcd.clear();
}

// prompts user input for current time and converts time to minutes
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

  firstNextFeeding();
}

// counts time and returns if it's a feeding time or not
bool time(){

  if(countTime == currentTime + 1440) {
    countTime = 0;
  }

  delay(60000); // count 1 minute
  countTime++;

  // loops through all target times
  for(int i = 0; i < numFeeding; i++) {

    if(countTime == targetTimes[i]) {
      nextFeeding(i);
      return true;
    }
  }
  return false;
}

// prints next feeding time in hours and minutes, after first feeding has already occurred
void nextFeeding(int i) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Next feeding");
  lcd.setCursor(0,1);
  lcd.print("time: ");
      
    if (targetTimes[i+1] == 0) {
      lcd.print((targetTimes[0] - (targetTimes[0]%60))/60);
      lcd.print(":");
      if((targetTimes[0]%60) < 10) {
        lcd.print("0");
        lcd.print(targetTimes[0]%60);
      } else {
          lcd.print(targetTimes[0]%60);
      }
    } else {
      lcd.print((targetTimes[i+1] - (targetTimes[i+1]%60))/60);
      lcd.print(":");
      if((targetTimes[i+1]%60) < 10) {
        lcd.print("0");
        lcd.print(targetTimes[i+1]%60);
      } else {
        lcd.print(targetTimes[i+1]%60);
      }
    }
}

// prints next feeding time before the first feeding has already occurred; must check the next closest feeding time regardless of order
void firstNextFeeding(){

  // display message
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Next feeding");
  lcd.setCursor(0,1);
  lcd.print("time: ");

  // declare local variables
  int nextTime = 1440;
  int a;

  // loops through current feeding times
  for (int i = 0; i < numFeeding; i++){
    if((targetTimes[i] - currentTime < nextTime) && (targetTimes[i] - currentTime >= 0)) {
      a = i;
      nextTime = targetTimes[i] - currentTime;
    }
  }

  lcd.print((targetTimes[a] - (targetTimes[a]%60))/60);
  lcd.print(":");
  if((targetTimes[a]%60) < 10) {
    lcd.print("0");
    lcd.print(targetTimes[a]%60);
  } else {
    lcd.print(targetTimes[a]%60);
  }
}

