#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_POTEN    4
#define PIN_FAN_PWM 25
#define CHAN_PWM     0

#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins

LiquidCrystal_I2C lcd(0x27,16,2); //initialize the LCD

// tacho defs
#define PIN_FAN_TACHO 34
#define RPM_CALC_PERIOD 1000
int fan1RPM = 0;
volatile int fan1InterruptCounter;
unsigned long previousmills;

void ICACHE_RAM_ATTR handleInterruptFan1Tacho()
{
  fan1InterruptCounter++;
}

 
void setup()
{
  Serial.begin(9600);
  Serial.println("starting..");
  // Our fan requests freq=25Khz. ESP32's spec for precision bound us to max of 11bits.
  // (max frequency is 80M/11=39K)
  ledcSetup(CHAN_PWM, 25000, 8); 
  ledcAttachPin(PIN_FAN_PWM, CHAN_PWM);

  pinMode(PIN_FAN_TACHO, INPUT_PULLUP);
  attachInterrupt(PIN_FAN_TACHO, handleInterruptFan1Tacho, FALLING);
  previousmills = millis();

  Wire.begin(SDA, SCL);           // attach the IIC pin
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  
  delay(500);
}
 
void loop()
{
 
  int adcVal = analogRead(PIN_POTEN); //read adc (12 bit)
  int pwmVal = map(adcVal, 0, 4095, 0, 255);  // re-map to pwmVal (8 bit)
  double potPct = adcVal / 4095.0 * 100;
  Serial.printf("ADC Val: %d, \t PWM: %d, \t pct: %.2f\n", adcVal, pwmVal, potPct);
  Serial.println(""); 

  lcd.setCursor(0, 0);              
  char buffer[16];
  sprintf(buffer, "PWM pct: %3.0f%%", potPct);
  lcd.print(buffer);        //display the Humidity on the LCD1602

  ledcWrite(CHAN_PWM, pwmVal);    // set the pulse width.

  if (millis() - previousmills > RPM_CALC_PERIOD) {
    int elapsedMs = millis() - previousmills;
    int count = fan1InterruptCounter;
    fan1InterruptCounter = 0;
    previousmills = millis();
    
    fan1RPM = count / 2 * elapsedMs / 1000;
    Serial.println(fan1RPM);

    lcd.setCursor(0, 1);
    char buffer[16];
    sprintf(buffer, "RPM: %8d", fan1RPM);
    lcd.print(buffer);

  }
 
  delay(1000);
}
