#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <PID_v1.h>
#include <WiFi.h>
#include <WebServer.h>

// Web Server AP
//const char* ssid = "smoken";
//const char* password = "12345678";
//IPAddress local_ip(192,168,1,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);

// Station
const char* ssid = "***REMOVED***";
const char* password = "***REMOVED***";

WebServer server(80);

// PID
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);

// thermocouple
int thermoSO = 19;
int thermoCS = 23;
int thermoSCK = 5;

MAX6675 thermocouple(thermoSCK, thermoCS, thermoSO);
 
// LCD
#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins
LiquidCrystal_I2C lcd(0x27,16,2); //initialize the LCD
#define NUM_SCREENS 2
char lcdBuffer[NUM_SCREENS][2][16];
int curScreen = 0;
unsigned long lcdMillis;

// fan
#define PIN_POTEN    4
#define PIN_FAN_PWM 25
#define CHAN_PWM     0

// tacho defs
#define PIN_FAN_TACHO 34
#define RPM_CALC_PERIOD 1000
int fan1RPM = 0;
volatile int fan1InterruptCounter;
unsigned long previousmills;

void IRAM_ATTR handleInterruptFan1Tacho()
{
  fan1InterruptCounter++;
}

void printLCD() {

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
 
void setup()
{
  Serial.begin(9600);
  Serial.println("starting..");
  // Our fan requests freq=25Khz. ESP32's spec for precision bound us to max of 11bits.
  // (max frequency is 80M/11^2=39K)
  ledcSetup(CHAN_PWM, 25000, 8); 
  ledcAttachPin(PIN_FAN_PWM, CHAN_PWM);

  pinMode(PIN_FAN_TACHO, INPUT_PULLUP);
  attachInterrupt(PIN_FAN_TACHO, handleInterruptFan1Tacho, FALLING);
  previousmills = millis();

  // LCD init
  Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lcdMillis = millis();

  Setpoint = 30;
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);

  //WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  //delay(100);

  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handle_OnConnect);
  //server.on("/led1on", handle_led1on);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  
  delay(300);
}
 
void loop()
{

  server.handleClient();

  Input = thermocouple.readCelsius();
  myPID.Compute();
 
  sprintf(lcdBuffer[0][0], "tgt:%3.0f cur:%3.0f", Setpoint, Input);
   
  int adcVal = analogRead(PIN_POTEN); //read adc (12 bit)
  Serial.printf("ADC Val: %d\n", adcVal);

  sprintf(lcdBuffer[0][1], "Man:%3.0f Aut:%3.0f", adcVal/4095.0*100, Output/255*100);

  int pwmVal=0;
  // set fan's PWM manually if adc>0, otherwise by PID
  if (adcVal>0) {
    pwmVal = map(adcVal, 0, 4095, 0, 255);  // re-map to pwmVal (8 bit)
  } else {
    pwmVal = (int) Output;
  }

  double pwmPct = pwmVal / 255.0 * 100;
  
  Serial.printf("PWM: %d, \t pct: %.2f\n", pwmVal, pwmPct);
  sprintf(lcdBuffer[1][0], "PWM pct: %3.0f%%", pwmPct);

  // set the PWM duty. fan specs states that min val is 30%.
  if (pwmPct>30) {
    ledcWrite(CHAN_PWM, pwmVal);
  } else {
   ledcWrite(CHAN_PWM, 0); 
  }

  // calc fan RPM
  if (millis() - previousmills > RPM_CALC_PERIOD) {
    int elapsedMs = millis() - previousmills;
    int count = fan1InterruptCounter;
    fan1InterruptCounter = 0;
    previousmills = millis();

    fan1RPM = (int) (count * 60.0 / 2 * 1000 / elapsedMs);
    Serial.println(fan1RPM);

    sprintf(lcdBuffer[1][1], "RPM: %8d", fan1RPM);
    //sprintf(buffer, "c: %8d", count);
  }

  printLCD();
 
  delay(1000);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Smoken PID control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Smoken Web Server</h1>\n";

  //ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  ptr +="<a class=\"button button-off\" href=\"/led2off\">Update</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
