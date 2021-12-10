#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <PID_v1.h>

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

  delay(500);
}
 
void loop()
{

  Input = thermocouple.readCelsius();
  myPID.Compute();
 
  sprintf(lcdBuffer[0][0], "tgt:%3.0f cur:%3.0f", Setpoint, Input);
   
  int adcVal = analogRead(PIN_POTEN); //read adc (12 bit)
  Serial.printf("ADC Val: %d\n", adcVal);

  sprintf(lcdBuffer[0][1], "Man:%3.0f Aut:%3.0f", adcVal/4095.0*100, Output/255*100);

  int pwmVal=0;
  // set fan's PWM manually if adc>0, otherwise by PID
  if (adcVal>100) {
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
