#include <ArduinoOTA.h>
#include <TimerMs.h>
#include <ESPRelay.h>
#include <ESP8266HTTPUpdateServer.h>
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsESP.h>



GyverDBFile db(&LittleFS, "/data.db");
SettingsESP sett("ESP configuration", &db);
ESPRelay Relay1;

String sw_version = "0.7.0";
String releaseDate = "01.08.2024";

#define WIFIAPTIMER 180000
#define RELAY_PIN 0


enum keys : size_t {
    deviceName = SH("deviceName"),
    
    relayInvertMode = SH("relayInvertMode"),
    saveRelayStatus = SH("saveRelayStatus"),
    relayStatus = SH("relayStatus"),

    homeKitSupport = SH("homeKitSupport"),
    homeKitPassword = SH("homeKitPassword"), 
};

void restart();
void factoryReset();

#include "wifi_func.h"
#include "homekit.h"
#include "settingsUI.h"
#include "function.h"


void setup() {
  Serial.begin(115200);

  Serial.println("");
  Serial.println("-----------------------------");
  Serial.println("Booting...");

  //Database setup
  dbSetup();

  //Relay setup
  Serial.println("Initialize relay");
  Relay1.SetPin(RELAY_PIN);
  Relay1.SetInvertMode(db[keys::relayInvertMode]);
  Relay1.ChangeStateCallback(ChangeRelayState);
  if(db[keys::saveRelayStatus]){ 
      Serial.println("Restore relay state");
      Relay1.SetState(db[keys::relayStatus]); };

  //WiFi setup
  wifiSetup(db[keys::deviceName]);

  //UI setup
  settingsSetup();

  //Enable OTA update
  Serial.println("Starting OTA updates");
  ArduinoOTA.begin();

  //HomeKit
  homekitSetup();

  Serial.println("Boot completed");
  Serial.println("-----------------------------");
}


void loop(){
  ArduinoOTA.handle();
  settingsLoop();
  db.tick();
  wifiLoop();
  homekitLoop();
}