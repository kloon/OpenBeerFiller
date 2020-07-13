#include "ReprapLCD.h"
#include "Config.h"
#include "InputConfig.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
char lines[4][21];

/* Setup Rotary Encoder */
#define ENCODER_OPTIMIZE_INTERRUPTS
Encoder rotEnc(ROT_ENC_A, ROT_ENC_B);
int oldPosition, newPosition = 0;
bool initializing = true;
int currentTrigger, triggerPosition, triggerTotal, buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void updateLine(int line, char output[]) {
  sprintf(lines[line], "%-20s", output);
}

void updateLCD() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0,i);
    lcd.print(lines[i]);
  }
}

void setupLCD() {
  lcd.init();
  lcd.backlight();
  updateLine(0, "Initialising...");
  pinMode(ROT_ENC_BUTTON, INPUT);
  triggerTotal = ((sizeof(FILL_SENSOR_TRIGGERS))/(sizeof(FILL_SENSOR_TRIGGERS[0])));
  triggerPosition = 0;
  oldPosition = FILL_SENSOR_TRIGGERS[triggerPosition];
  currentTrigger = FILL_SENSOR_TRIGGERS[triggerPosition];
}

void readRotEncRotation() {
  newPosition = rotEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition / 4;
    oldPosition += FILL_SENSOR_TRIGGERS[triggerPosition];
    
    if (oldPosition < 1)    { oldPosition = 0;    rotEnc.write(0); }
    if (oldPosition > 1022) { oldPosition = 1023; newPosition = 1023; rotEnc.write(1023); }
    currentTrigger = oldPosition;
  }
}

void readRotEncButton() {
  int reading = digitalRead(ROT_ENC_BUTTON);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle if the button state is LOW
      if (buttonState == LOW) {
        rotEnc.write(0);
        
        if (triggerPosition < triggerTotal-1) {
          triggerPosition += 1;
        }
        else {
          triggerPosition = 0;
        }

        currentTrigger = FILL_SENSOR_TRIGGERS[triggerPosition];
      }
    }
  }

  lastButtonState = reading;
}

int rotEncRead() {
  readRotEncButton();
  readRotEncRotation();
  return currentTrigger;
}
