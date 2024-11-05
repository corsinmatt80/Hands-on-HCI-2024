//IMPORT LIBRARIES:
#include <Servo.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

//BUZZERPIN:
int buzzerPin = 6;

//SERVO PRESETS:
#define PIN_SERVO 10
#define SERVO_OPEN 70
#define SERVO_CLOSED 150
Servo myServo;

//CODE PRESETS:
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
char reset = '#';
char enteredCode[5];
int currentCodeSize = 0;
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

//LEDPORTS:
int LEDROT = 7;
int LEDBLAU = 9;
int LEDGRUEN = 8;

//DISPLAY PRESETS:
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String oberstring;
String unterstring;

//POS = ANGLE OF TURNING
void moveGate(int pos) {
 myServo.attach(PIN_SERVO);
 myServo.write(pos);
 delay(500);
 myServo.detach();
}

void setup() {
 moveGate(SERVO_CLOSED);
 Serial.begin(9600);
 pinMode(LEDROT, OUTPUT);
 pinMode(LEDBLAU, OUTPUT);
 pinMode(LEDGRUEN, OUTPUT);
 lcd.begin(16,2);
 lcd.print("Enter Code!");
}

void loop() {
 //VIOLETT STATUS: AN ABER KEIN CODE EINGEGEBEN
 if(currentCodeSize == 0){
  analogWrite(LEDROT, 10);
  analogWrite(LEDBLAU, 10);
 }
 //GELB STATUS: AN UND ANGEFANGEN CODE EINZUGEBEN
 if(currentCodeSize >= 1 && currentCodeSize >= 4){
  analogWrite(LEDBLAU, 10);
  analogWrite(LEDGRUEN, 10);
 }


 char key = myKeypad.getKey();
 if (key == NO_KEY) return;
 
 //BEIM SCHLIESSEN CODE ZURÜCKSETZEN SONST CHARAKTER ZU CODE HINZUFÜGEN
 if (key == reset) {
  moveGate(SERVO_CLOSED);
  for(int i = 0; i < 4; i++){
    enteredCode[i] = -1;
  }
  currentCodeSize = 0;
  return;
 }else{
  tone(buzzerPin, 600, 50);   
  delay(100);                
  noTone(buzzerPin);          
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

 //WENN CODE RICHTIG IST, GRÜN LEUCHTEN UND TRESOR ÖFFNEN UND CODESIZE ZURÜCKSETZEN
 if (strcmp(enteredCode, code) == 0) {
  analogWrite(LEDGRUEN, LOW);
  oberstring = "CHUCHICHAESTLI";
  unterstring = "OEFFNET!";
  analogWrite(LEDGRUEN, 100);
  tone(buzzerPin, 500, 150);  
  delay(200);                     
  tone(buzzerPin, 700, 150);      
  delay(200);                    
  noTone(buzzerPin);              
  lcd.clear();
  for(int i = 0; i < oberstring.length(); i++){
    lcd.setCursor(0, 0);
    lcd.print(oberstring.substring(0,i+1));
    delay(200);
  }
  for(int i = 0; i < unterstring.length(); i++){
    lcd.setCursor(0, 1);
    lcd.print(unterstring.substring(0,i+1));
    delay(200);
  }
  for(int i = 0; i < 4; i++){
    enteredCode[i] = -1;
  }
  currentCodeSize = 0;
  lcd.setCursor(0,0);
  lcd.setCursor(0,0);
  moveGate(SERVO_OPEN);
  currentCodeSize = 0;
  while(myKeypad.getKey() != '#'){
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Code!");
 }



 //WENN CODE FALSCH IST, ROT LEUCHTEN UND TRESOR ZU LASSEN. EVENFALLS CODE RESETTEN
 if(currentCodeSize == 4 && strcmp(enteredCode, code) != 0){
  analogWrite(LEDROT,LOW);
  for (int i = 0 ; i<3; i++){
    tone(buzzerPin, 300, 150);
    delay(200);
  }
  analogWrite(LEDROT, 100);
  lcd.clear();
  oberstring = "CHUCHICHAESTLI";
  unterstring = "BLIBE ZUE!";
  for(int i = 0; i < oberstring.length(); i++){
    lcd.setCursor(0, 0);
    lcd.print(oberstring.substring(0,i+1));
    delay(200);
  }
  for(int i = 0; i < unterstring.length(); i++){
    lcd.setCursor(0, 1);
    lcd.print(unterstring.substring(0,i+1));
    delay(200);
  }
  lcd.setCursor(0,0);
  for(int i = 0; i < 4; i++){
    enteredCode[i] = -1;
  }
  while(myKeypad.getKey() != '#')
   delay(200);
 }
 currentCodeSize = 0;
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Enter Code!");
}