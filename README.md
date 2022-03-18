# Charcoal Smoker with Fan PID
A temprature PID controlled fan with ESP32. This project combines welding with IOT.

## Background


## Inventory List

|name|price|quantity|
|----|----|---|
|**Electronics**
|4p connector|2.6|10
|wire 4p|5|5m
|12v to 5v step down|4|4
|LCD 1602|2.4|1
|Esp 32|4|1
|3.3k resistor|1.5|300
|small bread board|4.3|3
|wires|3|120
|12v power supply|3.7|1
|thermocouple|5.3|2
|max6675|7.3|2
|12v blower fan 4pin|15.4|1
|**Mechanics**
|analog temperature gauge|4|1
|gas spring|15|2
|sealing tape|5|3.6m
|cover|12|
|**Equip**
|cutting discs|5|5
|sanding discs (40 grit)|13|5
|folding handles|5|2
|quick lock|4.5|2
|||
|**Total**|**122$**|

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

PCB:
- Used KiCAD
- nice refs: https://medium.com/invent-io/getting-started-with-embedded-systems-3-schematic-pcb-design-manufacturing-5566b2cf6963
- YouTube tutorial: https://www.youtube.com/watch?v=vaCVh2SAZY4
- download some CAD modules:
  - https://www.snapeda.com/home/
  - https://grabcad.com/library
- ordered PCB from: https://jlcpcb.com/

Gotchas:
- PWM's max freq/precision
- tacho with RC filter
- pin 4 as ADC with wifi.h bug (https://github.com/espressif/arduino-esp32/issues/102)