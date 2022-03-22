#include <ESPmDNS.h>
#include <AsyncWebConfig.h>
#include <pid.h>
#include <web_server.h>

String params = "["
  "{"
  "'name':'ssid',"
  "'label':'WLAN Name',"
  "'type':"+String(INPUTTEXT)+","
  "'default':''"
  "},"
  "{"
  "'name':'pwd',"
  "'label':'WLAN Password',"
  "'type':"+String(INPUTPASSWORD)+","
  "'default':''"
  "},"

  // PID
  "{"
  "'name':'manual',"
  "'label':'Manual PWM value',"
  "'type':"+String(INPUTNUMBER)+","
  "'min':0,'max':100,"
  "'default':'0'"
  "},"
  "{"
  "'name':'setpoint',"
  "'label':'Target temprature',"
  "'type':"+String(INPUTNUMBER)+","
  "'min':0,'max':300,"
  "'default':'100'"
  "},"
  "{"
  "'name':'fmin',"
  "'label':'Min PID Output',"
  "'type':"+String(INPUTNUMBER)+","
  "'min':0,'max':100,"
  "'default':'0'"
  "},"
  "{"
  "'name':'fmax',"
  "'label':'Max PID Output',"
  "'type':"+String(INPUTNUMBER)+","
  "'min':0,'max':100,"
  "'default':'100'"
  "},"
  "{"
  "'name':'kp',"
  "'label':'PID P',"
  "'type':"+String(INPUTFLOAT)+","
  "'min':0,'max':10,"
  "'default':'1'"
  "},"
  "{"
  "'name':'ki',"
  "'label':'PID I',"
  "'type':"+String(INPUTFLOAT)+","
  "'min':0,'max':10,"
  "'default':'0'"
  "},"
  "{"
  "'name':'kd',"
  "'label':'PID D',"
  "'type':"+String(INPUTFLOAT)+","
  "'min':0,'max':10,"
  "'default':'0'"
  "}"
  "]";

AsyncWebConfig conf;


void handleRoot(AsyncWebServerRequest *request) {
  conf.handleFormRequest(request);
  if (request->hasParam("SAVE")) {
    uint8_t cnt = conf.getCount();
    Serial.println("*********** Configuration ************");
    for (uint8_t i = 0; i<cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(" = ");
      Serial.println(conf.values[i]);
    }
    
    // update the PID
    setPIDconfs(conf.getInt("setpoint"), conf.getInt("fmin"), conf.getInt("fmax"),
              conf.getFloat("kp"), conf.getFloat("ki"), conf.getFloat("kd"));
  }
}

void initConfig() {
  Serial.println(params);
  conf.setDescription(params);
  conf.readConfig();
}

void initWebConfig() {
  //initWiFi();
  char dns[30];
  sprintf(dns,"%s.local",conf.getApName());
  if (MDNS.begin(dns)) {
    Serial.println("MDNS responder gestartet");
  }
  server.on("/conf",handleRoot);
}
