#include <Arduino.h>
#include <ESPWifi.hpp>
#include <ESPHttp.hpp>
#include <max6675.h>
#define START "start"
#define STOP "stop"
#define NONE "null"

const String UUID = "2ae09d7a-8f5d-496f-9917-4deabb3e5ad3";

const String WIFI_SSID = "Peppa Pig";
const String WIFI_PASSWORD = "LechonIsBack!2024";

const byte LED_RED = 13;
const byte LED_GREEN = 12;

const byte BTN_START = 18;
const byte BTN_STOP = 5;
const byte POTENTIOMETER = 35;

const byte MOTOR = 25;

const byte SCK_PIN = 14;
const byte CS_PIN = 27;
const byte SO_PIN = 26;


MAX6675 thermo(SCK_PIN, CS_PIN, SO_PIN);
ESPHttp http(UUID);

Sensor sensor;
String state = NONE;

void stopMachine();
void startMachine();


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

        if (http.createSensors()) {
            Serial.println("Successfully created machine instance.");
        } else {
            Serial.println("Duplicate machine instance.");
        }

        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);
        pinMode(MOTOR, OUTPUT);

        attachInterrupt(
            digitalPinToInterrupt(BTN_STOP), 
            stopMachine,
            FALLING 
        );

        attachInterrupt(
            digitalPinToInterrupt(BTN_START),
            startMachine, 
            FALLING
        );
    }
}

void loop(){

    if (WiFi.status() != WL_CONNECTED) return;

    if ( state.equals(START)) {
        digitalWrite(LED_GREEN, HIGH);

        while (state.equals(START)) {
            Serial.flush();

            int value = map(
                analogRead(POTENTIOMETER),
                0, 4095, 0, 255
            );  

            analogWrite(MOTOR, value);
            sensor.temperature = thermo.readCelsius();
            delay(10);

            http.updateSensors(sensor);
            sensor = http.retrieveSensors();

            if (sensor.is_stop) {
                state = STOP;
                break;
            }
        }
    }

    if (state.equals(STOP)) {
        analogWrite(MOTOR, 0);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        delay(1500);
        digitalWrite(LED_RED, LOW);
        state = NONE; 

        sensor.counter = 0;
        sensor.timer = 0;
        sensor.temperature = 0;

        http.updateSensors(sensor);
    }
}

void stopMachine() {
    if (state.equals(START)) {
       state = STOP;
       sensor.is_stop = true;
       sensor.is_start = false;
    }
}

void startMachine() {
    if (state.equals(NONE)) {
        state = START;
        sensor.is_start = true;
        sensor.is_stop = false;
    }
}