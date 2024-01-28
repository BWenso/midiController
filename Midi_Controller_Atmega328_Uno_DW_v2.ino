#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const int NButtons = 4;
const int buttonPin[NButtons] = {2, 3, 5, 7};
const int ledPin[NButtons] = {10, 11, 12, 13}; // Define LED pins
int buttonCState[NButtons] = {};
int buttonPState[NButtons] = {};
unsigned long lastDebounceTime[NButtons] = {0};
const unsigned long debounceDelay = 50;

byte midiCh = 1;
byte note = 36;
byte cc = 1;
int lastPressedButton = -1; // Variable to store the index of the last pressed button

void setup() {
  Serial.begin(115200);

  // Buttons and LEDs
  for (int i = 0; i < NButtons; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() {
  buttons();
}

void buttons() {
  for (int i = 0; i < NButtons; i++) {
    buttonCState[i] = digitalRead(buttonPin[i]);

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

        if (buttonCState[i] == LOW) {
          // Turn off all LEDs
          for (int j = 0; j < NButtons; j++) {
            digitalWrite(ledPin[j], LOW);
          }
          
          MIDI.sendNoteOn(note + i, 127, midiCh);
          digitalWrite(ledPin[i], HIGH); // Turn on the LED corresponding to the pressed button
          lastPressedButton = i; // Update the last pressed button
        } else {
          MIDI.sendNoteOn(note + i, 0, midiCh);
          // Do not turn off the LED when the button is released
          // Only turn off the LED when another button is pressed
        }
        buttonPState[i] = buttonCState[i];
      }
    }
  }
}