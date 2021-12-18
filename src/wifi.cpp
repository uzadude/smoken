#include <config.h>
#include <WiFi.h>
#include "lcd.h"


void initWIFI() {
      // Wifi
  //WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  //delay(100);

  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  sprintf(getLCDbuffer(0,0), "Wifi Connected");
  sprintf(getLCDbuffer(0,1), "%s", WiFi.localIP().toString().c_str());
  printLCD(0);
}