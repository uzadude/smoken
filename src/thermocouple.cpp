#include <config.h>
#include <max6675.h>

MAX6675 tc1(thermoSCK, thermoCS1, thermoSO);
MAX6675 tc2(thermoSCK, thermoCS2, thermoSO);

void initThermocouple(void) {
      // Thermocouples init
  digitalWrite(thermoCS1, LOW);
  digitalWrite(thermoCS2, LOW);
}

double readTC(int tcs) {
  digitalWrite(tcs, LOW);
  double ret = tc1.readCelsius();
  digitalWrite(tcs, HIGH);
  delay(100);
  return ret;
}

double readTC1() {
    return readTC(thermoCS1);
}

double readTC2() {
    return readTC(thermoCS2);
}
