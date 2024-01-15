// Food for Thought
// Eileen Kuang and Elaine Qian

// Include libraries for servo, lcd, and keypad
#include "Servo.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

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
  delay(2000);

  lcd.begin(16, 2); //sets up rows and columns on the screen
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Welcome to");
  lcd.setCursor(0,1);
  lcd.print(" Food 4 Thought!");

  myservo1.attach(50);
  // myservo1.write(100);
  // delay(3000);
  myservo1.write(90);

  myservo2.attach(51);
  // myservo2.write(100);
  // delay(3000);
  myservo2.write(85);

  myservo3.attach(52);
  myservo3.write(90);

  myservo4.attach(53);
  myservo4.write(90);

  delay(1500);
  lcd.clear();

}

void loop() {

  startingPage();

  while (wrong < 3){
    generateProblem();
    printProblem();
    if (getAns()) {
      dispense();
      wrong = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enjoy your snack!");
      delay(5000);
      startingPage();
    } else {
      wrong++;
    }
  }

  timeOut();
  wrong = 0;
}

void startingPage(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Press any key");
  lcd.setCursor(0,1);
  lcd.print("  to start :)");

  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }
  delay(200);
  lcd.clear();
  delay(300);
}

void generateProblem(){

  bool generate = true;

  while(generate){
    reseedRandom();

    op = (int) random(1, 5);

    if (op == 1)  {
      num1 = (int) random(10, 100);
      num2 = (int) random(10, 100);
      ans = num1 + num2;
    } else if (op == 2) {
      num1 = (int) random(10, 100);
      num2 = (int) random(10, 100);
      ans = num1 - num2;
    } else if (op == 3) {
      num1 = (int) random(2, 10);
      num2 = (int) random(5, 10);
      ans = num1 * num2;
    } else if (op == 4) {
      num1 = (int) random(10, 100);
      num2 = (int) random(2, 9);
      ans = (int) num1 / num2;
    }

    if(ans < 100 && ans > 10){
      if(op == 4){
        if(ans * num2 == num1){
          generate = false;
          lcd.print(num1);
          lcd.print(num2);

        }
      } else {
        generate = false;
        lcd.print(num1);
        lcd.print(num2);

      }
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
  delay(10);
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
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);

  if (input == ans){
    lcd.print("Correct!");
    delay(2000);
  } else {
    lcd.print("Incorrect >:((");
    delay(2000);
  }

  return(input == ans);

}

void dispense() {

  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Enter ur choice: ");

  // receive user input
  key = keypad.getKey();

  while(key == NO_KEY) {
    key = keypad.getKey();
  }

  choice = key;
  lcd.setCursor(0,1);
  lcd.print(choice);

    // dispense snack based on user choice
  switch(choice){

    case 'A':
      myservo1.write(100);
      delay(3000);
      myservo1.write(90);
      break;

    case 'B':
      myservo2.write(95);
      delay(3000);
      myservo2.write(85);
      break;

    case 'C':
      myservo3.write(100);
      delay(3000);
      myservo3.write(90);
      break;

    case 'D':
      myservo4.write(100);
      delay(3000);
      myservo4.write(90);
      break;
    }

}

void timeOut(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You're timed out!");
  delay(time);
}

void reseedRandom( void )
{
  static const uint32_t HappyPrime = 937;
  union
  {
    uint32_t i;
    uint8_t b[4];
  }
  raw;
  int8_t i;
  unsigned int seed;
  
  for ( i=0; i < sizeof(raw.b); ++i )
  {
    raw.b[i] = EEPROM.read( i );
  }

  do
  {
    raw.i += HappyPrime;
    seed = raw.i & 0x7FFFFFFF;
  }
  while ( (seed < 1) || (seed > 2147483646) );

  randomSeed( seed );  

  for ( i=0; i < sizeof(raw.b); ++i )
  {
    EEPROM.write( i, raw.b[i] );
  }
}