// --- thermocouple -----------------------------------------------------------------------------------------------------------------------

const int thermoSO = 19;
const int thermoSCK = 5;
const int thermoCS1 = 13;
const int thermoCS2 = 14;


// --- fan --------------------------------------------------------------------------------------------------------------------------------

// potentionmeter
#define PIN_POTEN    32 // (4) doesn't work with wifi.h
#define PIN_FAN_PWM 25
#define CHAN_PWM     0

// tacho defs
#define PIN_FAN_TACHO 34
#define RPM_CALC_PERIOD 1000


// --- LCD --------------------------------------------------------------------------------------------------------------------------------
// SDA/SCL must be connected to 21/22
#define NUM_SCREENS 2


// --- wifi --------------------------------------------------------------------------------------------------------------------------------

/// AP
//const char* ssid = "smoken";
//const char* password = "12345678";
//IPAddress local_ip(192,168,1,1);
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);

/// Station
const char* const ssid = "***REMOVED***";
const char* const password = "***REMOVED***";

