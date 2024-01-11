// Food for Thought
// Eileen Kuang and Elaine Qian

// Include libraries for servo, lcd, and keypad
#include "Servo.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

// Create servo objects:
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

// keypad
const int ROW_NUM = 4; // four rows
const int COLUMN_NUM = 4; // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {14, 15, 16, 17}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {18, 19, 20, 21}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM ); // initializing keypad object

// lcd display
const int RS = A0, EN = A1, D4 = A2, D5 = A3, D6 = A4, D7 = A5;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// piezo!
const int PIE = A15;

int num1, num2, op, ans, input;
int wrong=0; // counts consecutive wrong answers
char key, choice;
long time = 300000; // five minutes in milliseconds

void setup() {
  Serial.begin(9600);
  delay(1000);

  lcd.begin(16, 2); //sets up rows and columns on the screen
  lcd.clear();

  myservo1.attach(22);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Welcome to");
  lcd.setCursor(0,1);
  lcd.print(" Food 4 Thought!");

  myservo1.write(100);
  delay(3000);
  myservo1.write(90);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void generateProblem(){
  
  op = random(1, 5);

  while(true){
    num1 = random(10, 100);
    num2 = random(10, 100);

    if (op == 1)  {
      ans = num1 + num2;
    } else if (op == 2) {
      ans = num1 - num2;
    } else if (op == 3) {
      ans = num1 * num2;
    } else if (op == 4) {
      ans = (int) num1 / num2;
    }

    if(ans < 100 && ans > 10){
      if(op == 4){
        if(ans * num2 == num1){
          break;
        }
      } else
          break;
    }

  }

}

void printProblem(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("What is ");
  lcd.print(num1);
  if (op == 1){
      lcd.print("+");
    } else if (op == 2){
      lcd.print("-");
    } else if (op == 3){
      lcd.print("*");
    } else if (op == 4){
      lcd.print("/");
    }
  lcd.print(num2);
  lcd.print("?");
}

bool getAns() {

  lcd.setCursor(0,1);

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  input = 10*(key-'0');
  lcd.print(key);

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  input = input + (key-'0');
  lcd.print(key);

  return(input == ans);

  dispenseOrNot();

}

void dispenseOrNot() {

  // if user answers correctly
  if(getAns()){

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("Enter your choice: ");

    // receive user input
    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    choice = key;
    lcd.print(key);

    key = keypad.getKey();

    while(key == NO_KEY) {
      key = keypad.getKey();
    }

    input = input + (key-'0');

    lcd.setCursor(0,1);
    lcd.print(key);

    // dispense snack based on user choice
    switch(key){

      case 'A':
        myservo1.write(90);
        break;

      case 'B':
        myservo2.write(90);
        break;

      case 'C':
        myservo3.write(90);
        break;

      case 'D':
        myservo4.write(90);
        break;

    }

    wrong=0;

  }
  // if user's answer is incorrect
  else{

    wrong++;
    timeOut(wrong);

  }

}

void timeOut(int wrong){

  if(wrong > 3){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You're timed out!");
    delay(time);

  }

}