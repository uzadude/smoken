#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_POTEN    4
#define PIN_FAN_PWM 25
#define CHAN_PWM     0

#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins

LiquidCrystal_I2C lcd(0x27,16,2); //initialize the LCD
 
void setup()
{
  Serial.begin(9600);
  Serial.println("ohad test");
  ledcSetup(CHAN_PWM, 25000, 12);
  ledcAttachPin(PIN_FAN_PWM, CHAN_PWM);

  Wire.begin(SDA, SCL);           // attach the IIC pin
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  
  delay(500);
}
 
void loop()
{
 
  int adcVal = analogRead(PIN_POTEN); //read adc
  int pwmVal = adcVal;        // adcVal re-map to pwmVal
  Serial.print("ohadohad: ");
  Serial.println(pwmVal);

  lcd.setCursor(0, 0);              //set the cursor to column 0, line 1
  char buffer[16];
  sprintf(buffer, "ohadohad: %4d", pwmVal);
  lcd.print(buffer);        //display the Humidity on the LCD1602
  //lcd.print(pwmVal);   
  //lcd.setCursor(0, 1);              //set the cursor to column 0, line 0 
  //lcd.print("Humidity   :");        //display the Humidity on the LCD1602
  //lcd.print(DHT.humidity);  
  
  
  ledcWrite(CHAN_PWM, pwmVal);    // set the pulse width.
 
  delay(1000);
}
