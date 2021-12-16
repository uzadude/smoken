# Charcoal Smoker PID
A temprature PID controled fan with ESP32. 

## References
my kit:
https://www.amazon.com/Freenove-ESP32-WROVER-Contained-Compatible-Bluetooth/dp/B08FM2NCST


connect max6675:
https://github.com/adafruit/MAX6675-library/blob/master/examples/serialthermocouple/serialthermocouple.ino
http://www.esp32learning.com/code/esp32-and-max6675-example.php

multiple max6675:
https://arduino.stackexchange.com/questions/37193/multiple-3-wire-spi-sensor-interfacing-with-arduino
https://forum.arduino.cc/t/problem-with-multiple-max6675-thermocouple/573336/7

Fan specs:
https://www.mouser.co.il/datasheet/2/632/BFB1012EH-C18J-1602075.pdf

Connection picture:
https://github.com/KlausMu/esp32-fan-controller

PID:
https://playground.arduino.cc/Code/PIDLibrary/

nice ESP32 tutorials: https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/


Gotchas:
- PWM's max freq/precision
- tacho with RC filter
- pin 4 as ADC with wifi.h bug (https://github.com/espressif/arduino-esp32/issues/102)