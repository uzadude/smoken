#include <Arduino.h>
#include "config.h"
#include "lcd.h"
#include "pid.h"
#include <web_config.h>

double pwmPct=0;

void handlePWM() {
  int adcVal = analogRead(PIN_POTEN); //read adc (12 bit)
  //Serial.printf("ADC Val: %d\n", adcVal);

  sprintf(getLCDbuffer(0,1), "Man:%3.0f Aut:%3.0f", adcVal/4095.0*100, getPIDoutput());

  int pwmVal=0;
  // set fan's PWM manually if adc>0, otherwise by PID
  if (adcVal>0) {
    pwmVal = map(adcVal, 0, 4095, 0, 255);  // re-map to pwmVal (8 bit)
  } else if (conf.getInt("manual") > 0) {
    pwmVal = map(conf.getInt("manual"), 0, 100, 0, 255);
  } else {
    pwmVal = map((int) getPIDoutput(), 0, 100, 0, 255);
  }

  pwmPct = pwmVal / 255.0 * 100;
  
  //Serial.printf("PWM: %d, \t pct: %.2f\n", pwmVal, pwmPct);
  sprintf(getLCDbuffer(1,0), "PWM pct: %3.0f%%", pwmPct);

  // set the PWM duty. fan specs states that min val is 30%.
  if (pwmPct>30) {
    ledcWrite(CHAN_PWM, pwmVal);
  } else {
    ledcWrite(CHAN_PWM, 0); 
  }

}

int getPWM() {
  return (int) pwmPct;
}