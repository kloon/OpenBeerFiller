#include "ReprapLCD.h"
#include "Config.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
char lines[4][21];

/* Setup Rotary Encoder */
Encoder rotEnc(2, 3);
long oldPosition  = DEFAULT_FILL_SENSORS_TRIGGER;

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
}

int rotEncRead() {
  long newPosition = rotEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition + DEFAULT_FILL_SENSORS_TRIGGER;
    if (oldPosition < 1) { oldPosition = 0; }
    if (oldPosition > 1022) { oldPosition = 1023; newPosition = 1023; }
    return oldPosition;
  }
}
