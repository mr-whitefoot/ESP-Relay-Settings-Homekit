#include <arduino_homekit_server.h>


#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;

//static uint32_t next_heap_millis = 0;


//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on;	//sync the value
	LOG_D("Switch: %s", on ? "ON" : "OFF");
    Relay1.SetState( on );
}

void homekitSetup(){
	if (db[keys::homeKitSupport] == false){
		return;
	}

	  Serial.println("Homekit setup");

    //Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybe this is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".
	
    
    cha_switch_on.setter = cha_switch_on_setter;
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_switch_on.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void homekitLoop(){
	if (db[keys::homeKitSupport] == false){
		return;
	}
	
    arduino_homekit_loop();
	
	/*
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}*/
}

void homekitNotifyState(bool state){
  //homekit_characteristic_notify(&switch_state, switch_state.value);

}


