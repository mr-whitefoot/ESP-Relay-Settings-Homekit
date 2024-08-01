#include <TimerMs.h>

enum wifi : size_t {
    ssid = SH("ssid"),
    password = SH("password"),
    forceAP = SH("forceAP"),
};


TimerMs WiFiApTimer;

void wifiAPTimerHandler(){
    Serial.println("Restart ESP by AP timer");
    db[wifi::forceAP] = false;
    db.update();
    ESP.reset();
}


void wifiAp(const String& deviceName = "ESP"){
  Serial.println("Starting AP mode");

  String ssid = deviceName;
  ssid += " AP";
  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip.toString());

  // WiFiAP timer
  Serial.println("Starting WiFiAP timer");
  WiFiApTimer.setTime(WIFIAPTIMER);
  WiFiApTimer.setTimerMode();
  WiFiApTimer.attach(wifiAPTimerHandler);
  WiFiApTimer.start();
}


void wifiConnect(const String& deviceName = "ESP" ){
    WiFi.mode(WIFI_STA);
    WiFi.hostname(deviceName);
    
    Serial.println("Wifi connecting...");
    WiFi.begin(db[wifi::ssid], db[wifi::password]);
  
    int tries = 20;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print('.');
        if (!--tries){
            WiFi.mode(WIFI_AP);
            break;
        };
    };

    if (WiFi.status() == WL_CONNECTED){
        Serial.println();
        Serial.print("WiFi connected to SSID ");
        Serial.print(db[wifi::ssid]);
        Serial.println(" successfully");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        WiFi.setAutoReconnect(true);
        WiFi.persistent(true);
    } else {
        Serial.println();
        Serial.print("Can't connect to SSID ");
        Serial.print(db[wifi::ssid]);
        Serial.println(" reset to AP mode");
        db[wifi::forceAP] = true;
        db.update();
        ESP.reset();
    }
}


void wifiSetup(const String& deviceName = "ESP"){
    Serial.println("-----------------------------");
    Serial.println("Initialize WiFi");

    db.init(wifi::ssid, "");
    db.init(wifi::password, "");
    db.init(wifi::forceAP, false);

    // Connecting WiFi 
    Serial.print("WiFi force AP: ");;
    Serial.println(db[wifi::forceAP]);
    if (db[wifi::forceAP] == true ) {
        wifiAp(deviceName);
    } else {
        wifiConnect(deviceName);
    }
};


void wifiLoop(){
    WiFiApTimer.tick(); 
};


void wifiReset(){
    db[wifi::forceAP] = false;
    restart();
}