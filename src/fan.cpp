#include "config.h"
#include <esp32-hal-gpio.h>


// tacho
int fan1RPM = 0;
volatile int fan1InterruptCounter;
unsigned long previousmills;


void IRAM_ATTR handleInterruptFan1Tacho()
{
  fan1InterruptCounter++;
}

void initFan() {
  /// PWM
  // Our fan requests freq=25Khz. ESP32's spec for precision bound us to max of 11bits.
  // (max frequency is 80M/11^2=39K)
  ledcSetup(CHAN_PWM, 25000, 8); 
  ledcAttachPin(PIN_FAN_PWM, CHAN_PWM);
  ledcWrite(CHAN_PWM, 0); 

  // tacho
  pinMode(PIN_FAN_TACHO, INPUT_PULLUP);
  attachInterrupt(PIN_FAN_TACHO, handleInterruptFan1Tacho, FALLING);
  previousmills = millis();
}

int getFanRPM() {
  if (millis() - previousmills > RPM_CALC_PERIOD) {
    int elapsedMs = millis() - previousmills;
    int count = fan1InterruptCounter;
    fan1InterruptCounter = 0;
    previousmills = millis();

    fan1RPM = (int) (count * 60.0 / 2 * 1000 / elapsedMs);
  }
  
  return fan1RPM;
}