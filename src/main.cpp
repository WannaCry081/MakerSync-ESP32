#include <ESPWifi.hpp>


const String WIFI_SSID = "";
const String WIFI_PASSWORD = "";

const byte BTN_START = 13;
const byte BTN_STOP = 12;
const byte LED_GREEN = 14;
const byte LED_RED = 27;
const byte POTENTIOMETER = 26;
const byte MOTOR = 25;


ESPWifi wifi(WIFI_SSID, WIFI_PASSWORD);


void setup(){

    Serial.begin(115200);

    Serial.printf("Connecting to %s", WIFI_SSID);
    while (!wifi.isConnect()) {
        Serial.print(".");
        delay(1000);

        if (!wifi.isSuccess()){
            Serial.printf("\nError! Failed to Connect to%s.\n", WIFI_SSID);
        }
    }

    if (wifi.isConnect()){
        Serial.printf("\nSuccessfully Connected to %s\n", WIFI_SSID);
        wifi.displayStatus();
    }
}

void loop(){

    if (!wifi.isConnect()){
        return;
    }

    

}