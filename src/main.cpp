#include <config.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <thermocouple.h>
#include <fan.h>
#include <lcd.h>
#include <pid.h>
#include <wifi.h>
#include <web_server.h>
#include <pwm.h>


void setup()
{
  Serial.begin(9600);
  Serial.println("starting..");

  initThermocouple();

  initFan();

  initLCD();

  initPID(30);

  initWIFI();

  initWebServer();

  delay(300);
}
 
void loop()
{

  server.handleClient();

  double tmp1 = readTC1();
  updatePID(tmp1);

  double tmp2 = readTC2();
  sprintf(getLCDbuffer(0,0), "%3.0f %3.0f %3.0f", tmp1, tmp2, 30.0);
   
  handlePWM();

  int fan_rpm = getFanRPM();
  Serial.println(fan_rpm);
  sprintf(getLCDbuffer(1, 1), "RPM: %8d", fan_rpm);

  refreshLCD();
 
  delay(1000);
}
