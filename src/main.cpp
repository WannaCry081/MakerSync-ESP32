#include <Arduino.h>
#include <ESPWifi.hpp>
#include <max6675.h>


const String WIFI_SSID = "";
const String WIFI_PASSWORD = "";

const byte LED_RED = 13;
const byte LED_GREEN = 12;

const byte BTN_START = 19;
const byte BTN_STOP = 5;
const byte POTENTIOMETER = 35;

const byte MOTOR = 25;

const byte SCK_PIN = 14;
const byte CS_PIN = 27;
const byte SO_PIN = 26;

String state = "null";

MAX6675 thermo(SCK_PIN, CS_PIN, SO_PIN);


void emergencyStop();


void setup(){

    Serial.begin(115200);

    ESPWifi wifi(WIFI_SSID, WIFI_PASSWORD);
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

        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(MOTOR, OUTPUT);
        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);
    }

    attachInterrupt(
        digitalPinToInterrupt(BTN_STOP), 
        emergencyStop,
        FALLING 
    );
}

void loop(){

    int value = map(
        analogRead(POTENTIOMETER),
        0, 4095, 0, 255
    );

    if (WiFi.status() != WL_CONNECTED) return;

    if (state.equals("stop")){
        analogWrite(MOTOR, 0);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        delay(3000);
        digitalWrite(LED_RED, LOW);
        state = "null";
    }    

    if ((state.equals("null")) && 
        (digitalRead(BTN_START) == LOW)){
        digitalWrite(LED_GREEN, HIGH);
        state = "start";
    }

    if (state.equals("start")) {
        analogWrite(MOTOR, value);
        // thermo.readCelsius();
    } 
}

void emergencyStop() {
    state = "stop";
}