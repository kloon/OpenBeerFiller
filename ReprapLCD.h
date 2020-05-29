#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Encoder.h> // http://www.pjrc.com/teensy/td_libs_Encoder.html

void updateLine(int line, char output[]);
void updateLCD();
void setupLCD();
int rotEncRead();
