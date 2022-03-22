#include <WebServer.h>
#include <ESPmDNS.h>
#include <WebConfig.h>
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
  "'default':'2'"
  "},"
  "{"
  "'name':'ki',"
  "'label':'PID I',"
  "'type':"+String(INPUTFLOAT)+","
  "'min':0,'max':10,"
  "'default':'2'"
  "},"
  "{"
  "'name':'kd',"
  "'label':'PID D',"
  "'type':"+String(INPUTFLOAT)+","
  "'min':0,'max':10,"
  "'default':'2'"
  "}"
//   "{"
//   "'name':'setpoint',"
//   "'label':'Target temprature',"
//   "'type':"+String(INPUTRANGE)+","
//   "'min':0,'max':300,"
//   "'default':'100'"
//   "},"
//   "{"
//   "'name':'switch',"
//   "'label':'Schalter',"
//   "'type':"+String(INPUTCHECKBOX)+","
//   "'default':'1'"
//   "}"
  "]";

WebConfig conf;

// boolean initWiFi() {
//     boolean connected = false;
//     WiFi.mode(WIFI_STA);
//     Serial.print("Verbindung zu ");
//     Serial.print(conf.values[0]);
//     Serial.println(" herstellen");
//     if (conf.values[0] != "") {
//       WiFi.begin(conf.values[0].c_str(),conf.values[1].c_str());
//       uint8_t cnt = 0;
//       while ((WiFi.status() != WL_CONNECTED) && (cnt<20)){
//         delay(500);
//         Serial.print(".");
//         cnt++;
//       }
//       Serial.println();
//       if (WiFi.status() == WL_CONNECTED) {
//         Serial.print("IP-Adresse = ");
//         Serial.println(WiFi.localIP());
//         connected = true;
//       }
//     }
//     if (!connected) {
//           WiFi.mode(WIFI_AP);
//           WiFi.softAP(conf.getApName(),"",1);  
//     }
//     return connected;
// }

void handleRoot() {
  conf.handleFormRequest(&server);
  if (server.hasArg("SAVE")) {
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
  //server.begin(80);
}

// void loop() {
//   // put your main code here, to run repeatedly:
//   server.handleClient();
// //  MDNS.update();
// }