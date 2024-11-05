#include <Servo.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define PIN_SERVO 10
#define SERVO_OPEN 70
#define SERVO_CLOSED 150
Servo myServo;
char reset = '#';
char enteredCode[5];
int currentCodeSize = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
byte rowPins[KEYPAD_ROWS] = {29, 28, 27, 26};
byte colPins[KEYPAD_COLS] = {25, 24, 23, 22}; 
char keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
 {'D', 'C', 'B', 'A'},
 {'#', '9', '6', '3'},
 {'0', '8', '5', '2'},
 {'*', '7', '4', '1'}
};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

char code[] = { '1', '*', '3', '5', '\0' };  

void moveGate(int pos) {
 myServo.attach(PIN_SERVO);
 myServo.write(pos);
 delay(500);
 myServo.detach();
}

void setup() {
 moveGate(SERVO_CLOSED);
 Serial.begin(9600);
 lcd.begin(16,2);
 lcd.print("Enter Code!");
}

void loop() {
 char key = myKeypad.getKey();
 if (key == NO_KEY) return;
 Serial.print("Current key: ");
 Serial.println(key);
 if (key == reset) {
  moveGate(SERVO_CLOSED);
  for(int i = 0; i < 4; i++){
    enteredCode[i] = -1;
  }
  currentCodeSize = 0;
  return;
 }else{
  enteredCode[currentCodeSize] = key;
  if(currentCodeSize<4){
    Serial.print("Current size: ");
    Serial.println(currentCodeSize);
    currentCodeSize++;
  }
  enteredCode[currentCodeSize] = '\0'; 
  Serial.print("Current Code Entered: ");
  Serial.println(enteredCode);
 }

 if (strcmp(enteredCode, code) == 0) {
  lcd.clear();
  lcd.print("Success");
  lcd.setCursor(0,1);
  lcd.print("Tresor opens!");
  lcd.setCursor(0,0);
  moveGate(SERVO_OPEN);
  currentCodeSize = 0;
 }
 if(currentCodeSize == 4 && strcmp(enteredCode, code) != 0){
  lcd.clear();
  lcd.print("Declined!");
  lcd.setCursor(0,1);
  lcd.print("Stays shut!");
  lcd.setCursor(0,0);
  for(int i = 0; i < 4; i++){
    enteredCode[i] = -1;
  }
 }
}