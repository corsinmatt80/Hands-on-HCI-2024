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

// Configuration
bool isAlarmActivated = true; // Set to false to disable alarm trigger
int alarmTriggerThreshold = 6; // Change accepted distance here


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

//LEDPORTS:
int LEDROT = 7;
int LEDBLAU = 9;
int LEDGRUEN = 8;

//DISPLAY PRESETS:
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String oberstring;
String unterstring;

bool menuOpen = false;
bool boxOpen = false;
int menuIndex = 0;

//POS = ANGLE OF TURNING
void moveGate(int pos) {
 myServo.attach(PIN_SERVO);
 myServo.write(pos);
 delay(500);
 myServo.detach();
}

void setup() {
 lcd.begin(16,2);
 lcd.print("Starting...");
 moveGate(SERVO_CLOSED);
 Serial.begin(9600);
 pinMode(LEDROT, OUTPUT);
 pinMode(LEDBLAU, OUTPUT);
 pinMode(LEDGRUEN, OUTPUT);
 Serial.println(codeSize);
 pinMode(PIN_TRIGGER, OUTPUT);
 pinMode(PIN_ECHO, INPUT);
 showLEDScreen();
}

void showRGBLED(int red, int green, int blue) {
  analogWrite(LEDROT, red);
  analogWrite(LEDGRUEN, green);
  analogWrite(LEDBLAU, blue);
}

void checkRGBLED(){
  if(currentCodeSize >= 1 && currentCodeSize < 4){
    showRGBLED(0,10,10);
  }
  else if (currentCodeSize == 4 && boxOpen) {
    showRGBLED(0,10,0);
  }
  else if (currentCodeSize == 4 && boxOpen) {
    showRGBLED(10,0,0);
  }
  else {
    showRGBLED(0,0,10);
  }
}

void closeBox() {
  moveGate(SERVO_CLOSED);
  boxOpen = false;
}

void resetCode(){
  currentCodeSize = 0;
  closeBox();
  menuOpen = false;
  showLEDScreen();
}

void buttonSound(){
  tone(buzzerPin, 600, 50);   
  delay(100);                
  noTone(buzzerPin); 
}

void wrongButtonSound(){
  tone(buzzerPin, 300, 50);   
  delay(100);                
  noTone(buzzerPin); 
}

void showLEDScreen(){
  lcd.clear();
  if(menuOpen){
      lcd.setCursor(0,0);
      if(access == 0){
        lcd.print("No access registered.");
      } else {
        lcd.print(entryArray[index]);
      }
  } else if(!boxOpen) {
    lcd.setCursor(0,0);
    lcd.print("Enter code:");
    lcd.setCursor(0,1);
    if(currentCodeSize == 1){
      lcd.print("*");
    } else if(currentCodeSize == 2){
      lcd.print("**");
    } else if(currentCodeSize == 3){
      lcd.print("***");
    } else if(currentCodeSize == 4){
      lcd.print("****");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Hello");

    lcd.setCursor(0, 1);
    lcd.print(entryArray[index]);
  }
}

void loop() {
  checkAlarm();
  checkRGBLED();

  char key = myKeypad.getKey();
  if (key == NO_KEY) return;
 
 buttonSound();
 
 if (key == 'A') {
  menuOpen = !menuOpen;
  showLEDScreen();
 } else if (key == 'B') {
  if(menuOpen){
    if(menuIndex == 0) {
       wrongButtonSound(); 
       Serial.println("Error: Menu end reached.");
    } else {
      menuIndex--;
      showLEDScreen();
    }
  } else {
    wrongButtonSound(); 
    Serial.println("Error: Pressed B while menu was closed.");
  }
 } else if (key == 'C') {
  if(menuOpen){
    if(menuIndex +1 > access) {
       wrongButtonSound(); 
       Serial.println("Error: Menu end reached.");
    } else {
      menuIndex++;
      showLEDScreen();
    }
  } else {
    wrongButtonSound(); 
    Serial.println("Error: Pressed C while menu was closed.");
  }
 } else if (key == 'D') {
  wrongButtonSound(); 
  Serial.println("Error: Letter D not supported. ");

 } else if (key == '#') {
  wrongButtonSound(); 
  Serial.println("Error: Letter # not supported. ");
 }
  else if (key == '*') {
  resetCode();
 } else {          
  enteredCode[currentCodeSize] = key;
  if(currentCodeSize<4){
    currentCodeSize++;
  } 
  Serial.print("Current Code Entered: ");
  Serial.println(enteredCode);
  showLEDScreen();
  if(currentCodeSize == 4){
    checkCode();
  }
 }
}

void checkCode(){
  bool correctPin = false;
  for (int i = 0; i < codeSize; i++){
    if(compareCharArrays(enteredCode, codeArray[i].values)){
        moveGate(SERVO_OPEN);
        correctPin = true;
        boxOpen = true;

        addElement(codeArray[i].key);
        access++;

        showLEDScreen();

        // Open sound
        tone(buzzerPin, 500, 150);  
        delay(100);                     
        tone(buzzerPin, 700, 150);      
        delay(100);                    
        noTone(buzzerPin);              

        for(int i = 0; i < 4; i++){
          enteredCode[i] = -1;
        }
      }
    }
    //WENN CODE FALSCH IST, ROT LEUCHTEN UND TRESOR ZU LASSEN. EVENFALLS CODE RESETTEN
    if(!correctPin){
      
      numberMistakes++;
      showRGBLED(10,0,0);
      for (int i = 0 ; i<3; i++){
        tone(buzzerPin, 300, 150);
        delay(200);
      }
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unauthorized Ac-");
      lcd.setCursor(0, 1);
      lcd.print("cess Detected");

      if(numberMistakes >= 3){
        NGU();
        numberMistakes = 0;
      }
      Serial.print("Waiting start");
      
      for(int i = 0; i<20;i++){
        // TODO change block
        checkAlarm();
        delay(200);
      }
      Serial.print("Waiting stop");

      resetCode();
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

unsigned long lastCheckTime = 0;  // Variable to store the last time checkAlarm was called
const unsigned long checkInterval = 2500;  // Interval in milliseconds (5000 ms = 5 seconds)

void checkAlarm() {
  if (millis() - lastCheckTime >= checkInterval) {  // Check if 5 seconds have passed
    lastCheckTime = millis();  // Update the last check time
    alarm();
    
  }
}

void alarm(){
  if (isAlarmActivated) {
      digitalWrite(PIN_TRIGGER, LOW);
      delayMicroseconds(2);

      digitalWrite(PIN_TRIGGER, HIGH);
      delayMicroseconds(10);

      duration = pulseIn(PIN_ECHO, HIGH);
      distance = duration / 58;

      if (distance < SENSOR_MAX_RANGE && distance > alarmTriggerThreshold) {
        Serial.print("Too far away : ");
        Serial.println(distance);
        playNote(TONE_FST, 789);
        alarm();
      }
    }
}



