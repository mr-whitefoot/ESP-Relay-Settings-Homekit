void dbSetup(){
  Serial.println("-----------------------------");
  Serial.println("Initialize database:");
  LittleFS.begin();

  if (!db.begin()){
    Serial.println("WiFi database initialize error"); };

  db.init(keys::deviceName, "ESP Relay");
  db.init(keys::relayInvertMode, false);
  db.init(keys::saveRelayStatus, false);
  db.init(keys::relayStatus, false);
  db.init(keys::homeKitSupport, true);

  db.dump(Serial);
}

void factoryReset(){
  db.clear();
  db.update();
}

void restart(){
  Serial.println("Rebooting...");
  Serial.println("-------------------------------");
  ESP.restart();
}

void ChangeRelayState(){
  Serial.println("Change relay state triggered");
  if(db[keys::saveRelayStatus]){
    Serial.println("Save relay state");
    db[keys::relayStatus] = Relay1.GetState();
    db.update();
  }
}


