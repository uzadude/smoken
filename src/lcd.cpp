#include "config.h"
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>


// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
char lcdBuffer[NUM_SCREENS][2][16];

int curScreen = 0;
unsigned long lastPrintMS;

char* getLCDbuffer(int screen, int line) {
    return lcdBuffer[screen][line];
}

void initLCD() {
  //Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lastPrintMS = millis();
}

void printLCD(int screenNum) {
    lcd.setCursor(0, 0);              
    lcd.printf("%-16s", lcdBuffer[screenNum][0]);
    lcd.setCursor(0, 1);              
    lcd.printf("%-16s", lcdBuffer[screenNum][1]);
}

void refreshLCD() {

  if (millis() - lastPrintMS > 3000) {
   lastPrintMS = millis();

    printLCD(curScreen);
    Serial.println(lcdBuffer[curScreen][0]);
    Serial.println(lcdBuffer[curScreen][1]);

   curScreen++;
   curScreen = curScreen % NUM_SCREENS;
  }
  
}
