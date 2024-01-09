//Food for Thought
//Eileen Kuang and Elaine Qian

// Include libraries for servo, lcd, and keypad
#include "Servo.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

//need to make like 4 of these lolol
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

// lcd display
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// piezo!
const int PIE = 10;

int num1, num2, op, ans, input;
char key;

void setup() {
  Serial.begin(9600);
  delay(1000);

  lcd.begin(16, 2); //sets up rows and columns on the screen
  lcd.clear();

  myservo.attach(12);

  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void generateProblem(){
  
  op = random(1, 5);

  while(true){
    num1 = random(10, 100);
    num2 = random(10, 100);

    if (op == 1){
      ans = num1 + num2;
    } else if (op == 2){
      ans = num1 - num2;
    } else if (op == 3){
      ans = num1 * num2;
    } else if (op == 4){
      ans = (int) num1 / num2;
    }

    if(ans < 100 && ans > 10){
      if(op == 4){
        if(ans * num2 == num1){
          break;
        }
      } else {
        break;
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
}

bool getAns(){
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
}