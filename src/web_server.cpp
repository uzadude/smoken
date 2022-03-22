#include "FreeRTOS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <web_config.h>
#include <ArduinoJson.h>
#include <data.h>
#include "SPIFFS.h"

AsyncWebServer server(80);

void addArray(DynamicJsonDocument &doc, std::string name, double arr[], double factor) {
  JsonArray data = doc.createNestedArray(name);
  
  int tsize = getDataRowCount();
  for(int i = 0; i < tsize; i++){
      data.add((int)(arr[i] / factor));
  }
}

void addArray(DynamicJsonDocument &doc, std::string name, unsigned long arr[], double factor) {
  JsonArray data = doc.createNestedArray(name);
  
  int tsize = getDataRowCount();
  for(int i = 0; i < tsize; i++){
      data.add((int)(arr[i] / factor));
  }
}


String handle_data() {

  Serial.println("got data request");

  // trying to estimate the json size
  DynamicJsonDocument doc(getDataRowCount() * 200);
  dataMeasures dm = getMeasures();
  addArray(doc, "time_millis", dm.time_millis, 1);
  addArray(doc, "t1", dm.tmpsQ1, 1);
  addArray(doc, "t2", dm.tmpsQ2, 1);
  addArray(doc, "t3", dm.tmpsQ3, 1);
  addArray(doc, "pwm", dm.pwm, 1);
  addArray(doc, "rpm", dm.rpm, 1000);

  //doc["blah"] = 123;

  String output;
  serializeJson(doc, output);
  //serializeJson(doc, Serial);

  return output;
}

void handle_NotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void initWebServer() {

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("sending data response");
    request->send(200, "application/json", handle_data());
  });

  server.onNotFound(handle_NotFound);
  
  initWebConfig();

  server.begin();
  Serial.println("HTTP server started");
}
