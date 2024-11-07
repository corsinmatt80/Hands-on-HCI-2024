//IMPORT LIBRARIES:
#include <Servo.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

//DIEBSTAHLSCHUTZ
#define PIN_TRIGGER 37
#define PIN_ECHO 36
const int SENSOR_MAX_RANGE = 300; // in cm
unsigned long duration;
unsigned int distance;



//PIANO TÖNE
#define TONE_A 220
#define TONE_H 246
#define TONE_DST 293
#define TONE_AST 440
#define TONE_CST 277
#define TONE_EST 329
#define TONE_FST 369
#define TONE_A 220
#define TONE_A 220



//BUZZERPIN:
int buzzerPin = 6;

//SERVO PRESETS:
#define PIN_SERVO 10
#define SERVO_OPEN 70
#define SERVO_CLOSED 150
Servo myServo;

//
int numberMistakes = 0;

//CODE PRESETS:
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
char reset = '*';
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
struct Codes {
  String key;
  char values[5]; 
};
int access = 0;
int index = 0; //für access printing
Codes codeArray[] = {
  {"Corsin", {'1', '2', '3', '4', '\0'}},
  {"Marcel", {'9', '8', '7', '6', '\0'}},
  {"Andrin", {'4', '5', '6', '7', '\0'}}
};

const int codeSize = sizeof(codeArray) / sizeof(codeArray[0]);  
String* entryArray = nullptr;
int arraySize = 0;
void addElement(String element){
  String* newEntryArray = new String[arraySize + 1];
  for (int i = 0; i < arraySize;i++){
    newEntryArray[i] = entryArray[i];
  }
  newEntryArray[arraySize] = element;
  delete[] entryArray;
  entryArray = newEntryArray;
  arraySize++;
}
bool correctPin = false;

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
 Serial.println(codeSize);
 pinMode(PIN_TRIGGER, OUTPUT);
 pinMode(PIN_ECHO, INPUT);
}

void loop() {

 



 //VIOLETT STATUS: AN ABER KEIN CODE EINGEGEBEN
 if(currentCodeSize == 0){
  analogWrite(LEDROT, 10);
 }
 //GELB STATUS: AN UND ANGEFANGEN CODE EINZUGEBEN
 if(currentCodeSize >= 1 && currentCodeSize < 4){
  analogWrite(LEDBLAU, 10);
  analogWrite(LEDGRUEN, 10);
 }




 char key = myKeypad.getKey();
 if (key == NO_KEY) return;
 
 //BEIM SCHLIESSEN CODE ZURÜCKSETZEN SONST CHARAKTER ZU CODE HINZUFÜGEN
 if (key == reset) {
  Serial.println("CLOSE GATE");
  moveGate(SERVO_CLOSED);
  return;
 }else if(key == 'A' && access > 0){
  tone(buzzerPin, 600, 50);   
  delay(100);                
  noTone(buzzerPin); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(entryArray[index]);
  while (myKeypad.getKey() != '#'){
    if(myKeypad.getKey() == 'C'){
      tone(buzzerPin, 600, 50);   
      delay(100);                
      noTone(buzzerPin); 
     if(index < access - 1){
      Serial.println("DOWN");
      index++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(entryArray[index]);
     }
     }else if (myKeypad.getKey() == 'B'){
       tone(buzzerPin, 600, 50);   
       delay(100);                
       noTone(buzzerPin); 
       if(index > 0){
         index--;
         Serial.println("UP");
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print(entryArray[index]);
       }
     }
  }
   tone(buzzerPin, 600, 50);   
   delay(100);                
   noTone(buzzerPin); 
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Enter Code!");
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




 if(currentCodeSize == 4){
  for (int i = 0; i < codeSize; i++){
    if(compareCharArrays(enteredCode, codeArray[i].values)){
        moveGate(SERVO_OPEN);
        correctPin = true;
        Serial.print("Found code: ");
        Serial.println(codeArray[i].values);
        addElement(codeArray[i].key);
        access++;
        analogWrite(LEDGRUEN, LOW);
        oberstring = "Hallo";
        unterstring = codeArray[i].key;
        analogWrite(LEDGRUEN, 100);
        tone(buzzerPin, 500, 150);  
        delay(100);                     
        tone(buzzerPin, 700, 150);      
        delay(100);                    
        noTone(buzzerPin);              

        lcd.clear();
        for(int i = 0; i < oberstring.length(); i++){
          lcd.setCursor(0, 0);
          lcd.print(oberstring.substring(0,i+1));
          delay(200);
          alarmDetection();
        }
        for(int i = 0; i < unterstring.length(); i++){
          lcd.setCursor(0, 1);
          lcd.print(unterstring.substring(0,i+1));
          delay(200);
          alarmDetection();
        }
        for(int i = 0; i < 4; i++){
          enteredCode[i] = -1;
        }
        // Reset für das Display erst nach Schließen des Tresors
        while(myKeypad.getKey() != reset){
          delay(200);
          alarmDetection();
        }
        Serial.print(enteredCode);
        Serial.println(" - Code (hoffentlich reset)");
        // Nach dem Schließen zurück zur Eingabeaufforderung
        moveGate(SERVO_CLOSED);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter Code!");
        currentCodeSize = 0;
        analogWrite(LEDBLAU, LOW);
        analogWrite(LEDGRUEN, LOW);
        analogWrite(LEDROT, 10);
        }
    }
    //WENN CODE FALSCH IST, ROT LEUCHTEN UND TRESOR ZU LASSEN. EVENFALLS CODE RESETTEN
    if(!correctPin){
      numberMistakes++;
      analogWrite(LEDROT,LOW);
      for (int i = 0 ; i<3; i++){
        tone(buzzerPin, 300, 150);
        delay(200);
      }
      analogWrite(LEDROT, 100);
      lcd.clear();
      oberstring = "CHUCHICHAESTLI";
      unterstring = "BLIBT ZUE!";
      for(int i = 0; i < oberstring.length(); i++){
        lcd.setCursor(0, 0);
        lcd.print(oberstring.substring(0,i+1));
        delay(200);
        alarmDetection();
      }
      for(int i = 0; i < unterstring.length(); i++){
        lcd.setCursor(0, 1);
        lcd.print(unterstring.substring(0,i+1));
        delay(200);
        alarmDetection();
      }
      lcd.setCursor(0,0);
      for(int i = 0; i < 4; i++){
        enteredCode[i] = -1;
      }
      if(numberMistakes == 3){
        NGU();
        numberMistakes = 0;
      }
      
      for(int i = 0; i<10;i++){
        delay(400);
        alarmDetection();
      }

      currentCodeSize = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Code!");
    }
  correctPin = false;
  }
}

bool compareCharArrays(char* enteredCode, char* validCode){
    for (int i = 0; i < 4; i++) {
      if (enteredCode[i] != validCode[i]) {
       return false; 
     } 
  }
  return true;  
}

void playNote(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration*0.8); // kurze Pause zwischen den Noten
  noTone(buzzerPin);
}

void NGU(){
  playNote(TONE_A, 263);  // Never
  playNote(TONE_H, 263);  // Gonna
  playNote(TONE_DST,263);  // Give
  playNote(TONE_H, 263);  // You
  playNote(TONE_FST, 789);  // Up
  playNote(TONE_FST, 789);  // Up

  playNote(TONE_EST, 1578);  // Never
  playNote(TONE_A, 263);  // Never
  playNote(TONE_H, 263);  // Gonna
  playNote(TONE_DST,263);  // Give
  playNote(TONE_H, 263);  // You

  playNote(TONE_EST, 789);  // Never
  playNote(TONE_EST, 789);  // Gonna
  playNote(TONE_DST, 394);  // Run
  playNote(TONE_CST, 263);  // Around
  playNote(TONE_H, 600);  // And

  playNote(TONE_A, 263);  // Never
  playNote(TONE_H, 263);  // Gonna
  playNote(TONE_DST,263);  // Give
  playNote(TONE_H, 263);  // You

  playNote(TONE_DST, 1052);  // Gonna
  playNote(TONE_EST, 526);  // Make
  playNote(TONE_CST, 263);  // You
  playNote(TONE_H, 600);  // Cry

  playNote(TONE_A, 600);  // Never
  playNote(TONE_A, 300);  // Gonna
  playNote(TONE_A, 300);  // Say
  playNote(TONE_EST, 1052);  // Goodbye
  playNote(TONE_DST, 2105);  // Die
}

void alarmDetection(){
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration/58;

  if (distance > SENSOR_MAX_RANGE || distance <= 6){
  } else {
    Serial.print("Too far away : ");
    Serial.println(distance);
    playNote(TONE_FST, 789);
  }
}

