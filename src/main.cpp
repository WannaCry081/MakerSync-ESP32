#include <Arduino.h>
#include "constants.hpp"

// Define constants
#define START "start"
#define STOP "stop"
#define NONE "null"

// Define pin mappings
const byte LED_RED = 13;
const byte LED_YELLOW = 12; 
const byte LED_GREEN = 14;
const byte BTN_START = 19;
const byte BTN_STOP = 18;
const byte POTENTIOMETER = 35;
const byte MOTOR = 33;
const byte SCK_PIN = 27;
const byte CS_PIN = 26;
const byte SO_PIN = 25;

// Define Wi-Fi credentials
const String WIFI_SSID = "<WIFI NAME>";
const String WIFI_PASSWORD = "<WIFI PASSWORD>";

// Define UUID
const String UUID = "2ae09d7a-8f5d-496f-9917-4deabb3e5ad3";

// Initialize objects
LiquidCrystal_I2C lcd(0x27, 16, 2);
MAX6675 thermo(SCK_PIN, CS_PIN, SO_PIN);
ESPHttp http(UUID);

// Initialize global variables
Sensor sensor;
String state = NONE;

// Function prototypes
void stopMachine();
void startMachine();
void stopMachine();

void setup() {
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();

    // Connect to Wi-Fi
    ESPWifi wifi(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connecting to %s", WIFI_SSID);
    while (!wifi.isConnect()) {
        Serial.print(".");
        delay(1000);

        if (!wifi.isSuccess()) {
            Serial.printf("\nError! Failed to Connect to %s.\n", WIFI_SSID);
        }
    }

    if (wifi.isConnect()) {
        Serial.printf("\nSuccessfully Connected to %s\n", WIFI_SSID);
        wifi.displayStatus();

        // Create machine instance
        if (http.createSensors()) {
            Serial.println("Successfully created machine instance.");
        } else {
            Serial.println("Duplicate machine instance.");
        }

        // Initialize pin modes
        pinMode(LED_GREEN, OUTPUT);
        pinMode(LED_YELLOW, OUTPUT);
        pinMode(LED_RED, OUTPUT);
        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);
        pinMode(MOTOR, OUTPUT);

        // Turn off LEDs
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, HIGH);
        digitalWrite(LED_RED, HIGH);
        delay(1800);

        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, LOW);

        // Attach interrupts
        attachInterrupt(
            digitalPinToInterrupt(BTN_STOP), 
            stopMachine, FALLING);
        attachInterrupt(
            digitalPinToInterrupt(BTN_START), 
            startMachine, FALLING);    
    }    
}

void loop() {

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("No internet connection...");
        delay(1000);
        return;
    }

    // Check if the machine is initialized and started
    if (sensor.is_initialized && sensor.is_start) {
        digitalWrite(LED_YELLOW, HIGH);
        
        if (!state.equals(START)) return;

        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_GREEN, HIGH);

        // Run machine while the state is "START"
        while (state.equals(START)) {
            // Get motor speed from potentiometer
            int value = map(analogRead(POTENTIOMETER), 0, 4095, 0, 255);  
            analogWrite(MOTOR, value);
            delay(10);
 
            // Read temperature
            sensor.temperature = thermo.readCelsius();

            // Check if machine should stop
            if (sensor.is_stop) break;
            
            // Update sensor values
            http.updateSensors(sensor);       

            // Check if machine should stop
            if (sensor.is_stop) break;

            // Read sensor data
            sensor = http.retrieveSensors();

            // Check if machine should stop
            if (sensor.is_stop) break;
        }

    } else {
        sensor = http.retrieveSensors();
        return;
    }
        
    // Stop the machine
    if (state.equals(STOP) || sensor.is_stop) { // update the condition 

        analogWrite(MOTOR, 0);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        delay(1800);
        digitalWrite(LED_RED, LOW);

        // Reset sensor values
        state = NONE; 
        sensor.is_initialized = false;
        sensor.is_start = false;
        sensor.is_stop = false;
        http.updateSensors(sensor);
    }
}

// Interrupt service routine for stop button
void stopMachine() {
    if (state.equals(START)) {
        state = STOP;
        sensor.is_stop = true;
    }
}


void startMachine() {
    if (sensor.is_initialize && sensor.is_start)
        machine_state = START;
}
