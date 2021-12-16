#include "config.h"
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>


// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
char lcdBuffer[NUM_SCREENS][2][16];

int curScreen = 0;
unsigned long lcdMillis;

char* getLCDbuffer(int screen, int line) {
    return lcdBuffer[screen][line];
}

void initLCD() {
  //Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lcdMillis = millis();
}

void refreshLCD() {

  if (millis() - lcdMillis > 3000) {
   lcdMillis = millis();
   curScreen++;
   curScreen = curScreen % NUM_SCREENS;
  }
  
  lcd.setCursor(0, 0);              
  lcd.printf("%-16s", lcdBuffer[curScreen][0]);
  lcd.setCursor(0, 1);              
  lcd.printf("%-16s", lcdBuffer[curScreen][1]);

  Serial.println(lcdBuffer[curScreen][0]);
  Serial.println(lcdBuffer[curScreen][1]);
}
