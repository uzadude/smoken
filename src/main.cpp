#include <config.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <thermocouple.h>
#include <fan.h>
#include <lcd.h>
#include <pid.h>
#include <wifi.h>
#include <web_server.h>
#include <web_config.h>
#include <pwm.h>
#include <data.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("starting..");

  initFan();
  initData();
  initLCD();

  initConfig();
  initWIFI();
  initWebServer();

  initThermocouple();

  setPIDconfs(conf.getInt("setpoint"), conf.getInt("fmin"), conf.getInt("fmax"),
              conf.getFloat("kp"), conf.getFloat("ki"), conf.getFloat("kd"));
  //setPIDconfs(30, 0, 255, 2, 5, 1);

  delay(2000);
}
 
void loop()
{

  server.handleClient();

  double tmp1 = readTC(0);
  updatePID(tmp1);

  double tmp2 = readTC(1);
  sprintf(getLCDbuffer(0,0), "%3.0f %3.0f %3d", tmp1, tmp2, conf.getInt("setpoint"));

  handlePWM();

  int fan_rpm = getFanRPM();
  sprintf(getLCDbuffer(1, 1), "RPM: %8d", fan_rpm);

  addReads(tmp1, tmp2, (double) conf.getInt("setpoint"), getPWM(), fan_rpm);

  refreshLCD();
 
  delay(1000);
}
