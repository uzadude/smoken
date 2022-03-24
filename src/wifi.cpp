#include <config.h>
#include <WiFi.h>
#include "lcd.h"
#include <web_config.h>
#include <ESPmDNS.h>

void begin() {
  WiFi.begin(conf.values[0].c_str(),conf.values[1].c_str());
}

void WiFi_Connected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void Get_IPAddress(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WIFI is connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void Wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WIFI access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Reconnecting...");
  begin();
}

void initWIFI() {
  boolean connected = false;
  const char * wifi_str;

  WiFi.mode(WIFI_STA);
  Serial.print("connecting to: ");
  Serial.print(conf.values[0]);
  Serial.println(" network");

  sprintf(getLCDbuffer(0,0), "connecting to: ");
  sprintf(getLCDbuffer(0,1), "%s", conf.values[0]);
  printLCD(0);

  if (conf.values[0] != "") {
    WiFi.disconnect(true);
    delay(1000);
    WiFi.onEvent(WiFi_Connected,SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(Get_IPAddress, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 

    begin();
    uint8_t cnt = 0;
    while ((WiFi.status() != WL_CONNECTED) && (cnt<10)){
      delay(1000);
      Serial.print(".");
      cnt++;
      sprintf(getLCDbuffer(0,0), "%s", conf.values[0]);
      sprintf(getLCDbuffer(0,1), std::string(cnt, '.').c_str());
      printLCD(0);
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("IP-Address = ");
      Serial.println(WiFi.localIP());
      wifi_str = WiFi.localIP().toString().c_str();
      connected = true;
    }
  }
  if (!connected) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(conf.getApName(), "12345678");
    delay(100);
    Serial.print("Started AP: ");
    Serial.println(conf.getApName());
    wifi_str = WiFi.softAPIP().toString().c_str();
    Serial.print("IP-Address AP WIFI= ");
    Serial.println(WiFi.softAPIP());
  }

  sprintf(getLCDbuffer(0,0), "Wifi is up");
  sprintf(getLCDbuffer(0,1), "%s", wifi_str);
  printLCD(0);
}