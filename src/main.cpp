#include <Arduino.h>
#include <ESPScreen.hpp>
#include "const.hpp"


void stopExecution();
void startMachine();
void stopMachine();

void setup() {
    
    Serial.begin(115200);
    initLCD();

    ESPWifi wifi(WIFI_SSID, WIFI_PASSWORD);    
    displayLCD("Connecting to",
                WIFI_SSID.c_str());

    while (!wifi.isConnect()) {
        delay(1000);

        if (!wifi.isSuccess()) {
            Serial.printf("\nError connecting to %s...\n", WIFI_SSID);
            displayLCD("Network Error :<",
                       "Please try again");
        }
    }

    if (wifi.isConnect()) {
        displayLCD(" WIFI Connected ",
                    "Happy Extruding!!"); 

        if (http.createMachine()){
            sensor = http.createSensors();
        }

        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);
        pinMode(IR_PIN, INPUT);

        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_YELLOW, OUTPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(RELAY_MODULE, OUTPUT);

        attachInterrupt(digitalPinToInterrupt(BTN_START), 
            startMachine, RISING);

        attachInterrupt(digitalPinToInterrupt(BTN_STOP), 
            stopMachine, RISING);
    }


    digitalWrite(RELAY_MODULE, LOW);

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, HIGH); 
    delay(2000);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW); 
    delay(2000);
}

void loop() {

    if (WiFi.status() != WL_CONNECTED) {
        displayLCD("Network Issue :(",
                    "Low Signal Rate!");
        return;
    }

    sensor = http.retrieveSensors();

    if (machine_state.equals(STOP) || sensor.is_stop ) {
        stopExecution();
        return;
    }

    if (sensor.is_initialize && sensor.is_start) {
        digitalWrite(LED_YELLOW, HIGH);
        displayLCD("To Start, Press ",
                    "White Button -->");

        if (machine_state.equals(NONE)) 
            return;

        if (machine_state.equals(START)) {
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(RELAY_MODULE, HIGH);
        }

        while (machine_state.equals(START)) {
            
            sensor = http.retrieveSensors();
            displayLCD("  Extruding...",
                        " Percent: " + sensor.counter);
            
            sensor.temperature = thermo.readCelsius();

            if (machine_state.equals(STOP) 
                || sensor.is_stop
                || (digitalRead(IR_PIN) == LOW) ) {
                    Serial.printf("Stopping...");
                    stopExecution();
                    return;
                }

            http.updateSensors(sensor);

            if (machine_state.equals(STOP) 
                || sensor.is_stop
                || (digitalRead(IR_PIN) == LOW) ) {
                    Serial.printf("Stopping...");
                    stopExecution();
                    return;
                }

            delay(100);
        }
    } else {
        displayLCD("Please Select an",
                    "Option via App:>");
    }
    delay(100);
}


void startMachine() {
    if (sensor.is_initialize && sensor.is_start)
        machine_state = START;
}


void stopMachine() {
    if (sensor.is_initialize && sensor.is_start)
        machine_state = STOP;
}


void stopExecution() {
    char * line1 = "";
    char * line2 = "";

    if (digitalRead(IR_PIN) == LOW){
        line1 = "Process Broken!!";
        line2 = "Machine has Stop";
    } else {
        line1 = "Emergency Button";  
        line2 = "has been Pressed"; 
    }

    displayLCD(line1, line2);
    
    if (sensor.is_initialize) {
        sensor.is_initialize = false;
    }

    sensor.is_start = false;
    sensor.is_stop = true;
    sensor.time = 0;
    sensor.counter = 0;  

    machine_state = NONE;

    digitalWrite(RELAY_MODULE, LOW);    
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    http.updateSensors(sensor);

    if (digitalRead(IR_PIN) == LOW) {
        http.createNotifications(
            "Petamentor has stopped due to some issues.",
            "Check and fix the machine's problems before proceeding."
        );
    } else {
        http.createNotifications(
            "Petamentor's emergency stop has been activated.",
            "The emergency button has been pressed on the machine. Petamentor has stopped."
        );
    }

    delay(5000);

    sensor.is_stop = false;
    http.updateSensors(sensor);
    digitalWrite(LED_RED, LOW);
    displayLCD("Thanks for using",
                ">> MakerSync! <<"); 

    delay(5000);
}