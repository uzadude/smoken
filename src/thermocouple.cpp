#include <config.h>
#include <max6675.h>

int tcCS[]  = {thermoCS1, thermoCS2};

MAX6675 tc[] = {
  MAX6675(thermoSCK, tcCS[0], thermoSO),
  MAX6675(thermoSCK, tcCS[1], thermoSO),
};

void initThermocouple(void) {
      // Thermocouples init
  digitalWrite(thermoCS1, LOW);
  digitalWrite(thermoCS2, LOW);
}

double readTC(int i) {
  digitalWrite(tcCS[i], LOW);
  double ret = tc[i].readCelsius();
  digitalWrite(tcCS[i], HIGH);
  delay(100);
  return ret;
}
