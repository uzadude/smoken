#include <config.h>
#include <WiFi.h>
#include "lcd.h"
#include <web_config.h>
#include <ESPmDNS.h>

void initWIFI() {
  boolean connected = false;
  const char * wifi_str;

  WiFi.mode(WIFI_STA);
  Serial.print("connecting to: ");
  Serial.print(conf.values[0]);
  Serial.println(" network");

  if (conf.values[0] != "") {
    WiFi.begin(conf.values[0].c_str(),conf.values[1].c_str());
    uint8_t cnt = 0;
    while ((WiFi.status() != WL_CONNECTED) && (cnt<10)){
      delay(500);
      Serial.print(".");
      cnt++;
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