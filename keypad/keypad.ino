#include <Servo.h>
#include <Key.h>
#include <Keypad.h>

#define PIN_SERVO 10
#define SERVO_OPEN 0
#define SERVO_CLOSED 90
Servo myServo;
int lastPos;

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

char code[] = { '1', '*', '3', '5' }; // gewählter code "1*35"
int codeLen = 4; // länge des codes
int codePos = 0; // anzahl angegebene richtige stellen im code

void moveGate(int pos) {
 // verbinden mit der servo
 myServo.attach(PIN_SERVO); // servo zu position bewegen
 myServo.write(pos);
 // speichere die position
 lastPos = pos;
 // warte 500ms
 delay(500);
 // löse verbindung auf
 myServo.detach();
}
void setup() {
 // initialisiere das gate als "zu"
 moveGate(SERVO_CLOSED);
 Serial.begin(9600);
}
void loop() {
 // letzter gespresster charakter speichern
 char key = myKeypad.getKey();
 
 // mach nichts wenn kein charakter gedrückt wird
 if (key == NO_KEY) return;
 Serial.print(key);
 // Close lock if it is open and key was pressed
 if (lastPos == SERVO_OPEN) {
  moveGate(SERVO_CLOSED);
  return;
 }
 // Process key press
 if (key == code[codePos]) {
  // wenn der teil vom schlüssel korrekt gedrückt wird, schiebe position eines hoch
  codePos++;
 }
 else {
  // falscher charakter : wieder bei 0 starten
  codePos = 0;
  // todo: FEHLERMELDUNG und ggf. LED
 }
 // richtiger Code eingegeben
 if (codePos == codeLen) {
  // Gate öffnen
  moveGate(SERVO_OPEN);
  // cursor wieder zurück
  codePos = 0;
 }
}