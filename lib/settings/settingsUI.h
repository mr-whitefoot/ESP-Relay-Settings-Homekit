#pragma once
#include <StringUtils.h>


enum buttons : size_t {
    saveRestart = SH("saveRestart"),
    save = SH("save"),
};

void build(sets::Builder& b) {
    {   
        sets::Menu g(b, "WiFi configuration");
        b.Input(wifi::ssid, "SSID");
        b.Pass(wifi::password, "Password");
        if (b.Button(buttons::saveRestart, "Save & Restart")) {
            db[wifi::forceAP] = false;
            db.update();
            wifiReset();
        }
    }

    
        if (b.beginMenu("Homekit configuration")){
            //b.Switch(keys::homeKitSupport, "HomeKit support");
            if (b.Button("homeKitResetPairing"_h, "Reset pairing & Restart")){
                homekit_storage_reset();
                restart();
            }

            if (b.Button(buttons::saveRestart, "Save & Restart")){
                db.update();
                restart();
            }
            b.endMenu();
        }


        if (b.beginMenu("Relay configuration")){
            b.Switch(keys::relayInvertMode, "Relay invert mode");
            b.Switch(keys::saveRelayStatus, "Save realy status");
            
            if (b.Button(buttons::save, "Save")){
                db.update();
            }
            b.endMenu();
        }


        if (b.beginMenu("Service")){
            
            if (b.Button("dbDump"_h, "Database dump to serial port")){
                db.dump(Serial); }

            if (b.Button("dbClear"_h, "Drop database")){
                db.clear();
                db.update(); }

            if (b.Button("factoryReset"_h, "Factory reset")){
                factoryReset();
                restart(); }

            b.endMenu();
        }

        if (b.beginGroup("Information")) {
            b.Label("homeKitPassword"_h, "HomeKit password", "1111-1111");
            b.Label("version"_h, "Version", sw_version);
            b.Label("releaseDate"_h, "Release date", releaseDate);

            b.endGroup();  // закрыть группу
        }
}


void update(sets::Updater& upd) {
    //upd.update(keys::homeKitSupport, random(100));
    //upd.update("lbl2"_h, millis());
}


void settingsSetup(){
    Serial.println("-----------------------------");
    Serial.println("Settings setup");
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);
}


void settingsLoop() {
    sett.tick();
}