#include <Key.h>
#include <Keypad.h>
// Define number of rows and columns
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

byte rowPins[KEYPAD_ROWS] = {9, 8, 7, 6}; // Rows 0 to 3
byte colPins[KEYPAD_COLS] = {5, 4, 3, 2}; // Columns 0 to 3

char keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
 {'D', 'C', 'B', 'A'},
 {'#', '9', '6', '3'},
 {'0', '8', '5', '2'},
 {'*', '7', '4', '1'}
};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);


void setup() {
  Serial.begin(9600);
}
void loop() {
   // Get pressed key
 char key = myKeypad.getKey();
 // Do nothing if no key was pressed
 if (key == NO_KEY) return;
 // Print pressed key to serial
 Serial.print(key);

}