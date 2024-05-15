#include <Arduino.h>
#include "constants.hpp"


void startMachine();
void stopMachine();

void setup() {
    Serial.begin(115200);

    ESPWifi wifi(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connecting to %s...", WIFI_SSID);
    while (!wifi.isConnect()) {
        Serial.print(".");
        delay(1000);

        if (!wifi.isSuccess()) {
            Serial.printf("\nError connecting to %s...\n", WIFI_SSID);
        }
    }

    if (wifi.isConnect()) {

        if (http.createMachine()){
            sensor = http.createSensors();
            Serial.println("\nSuccessfully created machine instance...\n");
        } else {
            Serial.println("\nDuplicate instance of machine...\n");
        }

        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);

        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_YELLOW, OUTPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(RELAY_MODULE, OUTPUT);

        attachInterrupt(digitalPinToInterrupt(BTN_START), 
            startMachine, RISING);

        attachInterrupt(digitalPinToInterrupt(BTN_STOP), 
            stopMachine, RISING);


        sensor.is_start = false;
        sensor.is_initialize = false;
        sensor.is_stop = false;
        sensor.counter = 0;
        sensor.time = 0;       

        digitalWrite(RELAY_MODULE, LOW); 
    }   
}

void loop() {

    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.printf("No wifi connected...\n");
        return;
    }

    sensor = http.retrieveSensors();

    if (machine_state.equals(STOP) || sensor.is_stop) {
        Serial.println("Machine has stopped");
        
        sensor.is_start = false;
        sensor.is_stop = false;
        sensor.is_initialize = false;
        sensor.counter = 0;
        sensor.time = 0;

        machine_state = NONE;
        
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);

        digitalWrite(RELAY_MODULE, LOW);

        http.updateSensors(sensor);
        // http.createNotifications(
        //     "",
        //     ""
        // );
        
        digitalWrite(LED_RED, LOW);
        return;
    }

    if (sensor.is_start && sensor.is_initialize) {
        Serial.println("Waiting for button pressed");
        sensor.temperature = thermo.readCelsius();   
        Serial.println(sensor.temperature);

        if (machine_state.equals(NONE)){
            Serial.println("YELLOW IS ON");
            digitalWrite(LED_YELLOW, HIGH);
            return;
        }

        if (machine_state.equals(START)) {

            Serial.println("Machine has started");

            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, HIGH);

            digitalWrite(RELAY_MODULE, HIGH);

            Sensor newSensor = http.retrieveSensors();

            if (newSensor.is_stop || machine_state.equals(STOP)) 
                sensor.is_stop = newSensor.is_stop;

            http.updateSensors(sensor);
            return;
        }
    }

    delay(10);
}


void startMachine() {
    if (sensor.is_initialize && sensor.is_start)
        machine_state = START;
}

void stopMachine() {
    if (sensor.is_initialize && sensor.is_start)
        machine_state = STOP;
}