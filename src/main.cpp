#include <Arduino.h>
#include <ESPWifi.hpp>
#include <max6675.h>


const String WIFI_SSID = "";
const String WIFI_PASSWORD = "";

const byte BTN_START = 13;
const byte BTN_STOP = 12;
const byte LED_GREEN = 14;
const byte LED_RED = 27;
const byte POTENTIOMETER = 26;
const byte MOTOR = 25;
const byte SCK_PIN = 14;
const byte CS_PIN = 27;
const byte SO_PIN = 26;

int state = 0;

MAX6675 thermo(SCK_PIN, CS_PIN, SO_PIN);

void emergencyStop();
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

        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_RED, OUTPUT);
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

    if (!wifi.isConnect()){
        return;
    }

    if ((state == 0) && (digitalRead(BTN_START) == LOW)){
        state = 1;
        digitalWrite(LED_GREEN, HIGH);
    }

    if (state == 1) {

        int value = map(
            analogRead(POTENTIOMETER),
            0, 4095, 
            0, 255
        );

        analogWrite(MOTOR, value);
    } 

    if (state == -1){
        analogWrite(MOTOR, 0);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        delay(5000);
        digitalWrite(LED_RED, LOW);
        delay(5000);

        state = 0;
    }
}

void emergencyStop() {
    state = -1;
}