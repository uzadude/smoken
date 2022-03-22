#include <WebServer.h>
#include <web_config.h>
#include <ArduinoJson.h>
#include <data.h>
#include "SPIFFS.h"

WebServer server(80);

void handle_OnConnect() {
  File htmlFile = SPIFFS.open("/index.html", "r");
  if (htmlFile) {
    server.send(200, "text/html", htmlFile.readString()); 
    htmlFile.close();
  }
}

void addArray(DynamicJsonDocument &doc, std::string name, std::deque<double> dq, double factor) {
  JsonArray data = doc.createNestedArray(name);
  
  int tsize = dq.size();
  for(int i = 0; i < tsize; i++){
      data.add((int)(dq.at(i) / factor));
  }
}

void handle_data() {
  //StaticJsonDocument<1024> doc;
  DynamicJsonDocument doc(4096);
  dataMeasures dm = getMeasures();
  addArray(doc, "t1", dm.tmpsQ1, 1);
  addArray(doc, "t2", dm.tmpsQ2, 1);
  addArray(doc, "t3", dm.tmpsQ3, 1);
  addArray(doc, "pwm", dm.pwm, 1);
  addArray(doc, "rpm", dm.rpm, 1000);

  //doc["blah"] = 123;

  String output;
  serializeJson(doc, output);
  //serializeJson(doc, Serial);

  server.send(200, "application/json", output); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void initWebServer() {

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", handle_OnConnect);
  server.on("/data", handle_data);
  server.onNotFound(handle_NotFound);
  
  initWebConfig();

  server.begin();
  Serial.println("HTTP server started");
}
