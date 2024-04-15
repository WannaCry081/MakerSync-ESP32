#include <Arduino.h>
#include <ESPWifi.hpp>
#include <ESPHttp.hpp>
#include <max6675.h>

// Define constants
#define START "start"
#define STOP "stop"
#define NONE "null"

// Define pin mappings
const byte LED_RED = 13;
const byte LED_GREEN = 12;
const byte BTN_START = 18;
const byte BTN_STOP = 5;
const byte POTENTIOMETER = 35;
const byte MOTOR = 25;
const byte SCK_PIN = 14;
const byte CS_PIN = 27;
const byte SO_PIN = 26;

// Define Wi-Fi credentials
const String WIFI_SSID = "<WIFI NAME>";
const String WIFI_PASSWORD = "<WIFI PASSWORD>";

// Define UUID
const String UUID = "2ae09d7a-8f5d-496f-9917-4deabb3e5ad3";

// Initialize objects
MAX6675 thermo(SCK_PIN, CS_PIN, SO_PIN);
ESPHttp http(UUID);

// Initialize global variables
Sensor sensor;
String state = NONE;

// Function prototypes
void stopMachine();
void startMachine();

void setup() {
    Serial.begin(115200);

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
        pinMode(LED_RED, OUTPUT);
        pinMode(BTN_START, INPUT_PULLUP);
        pinMode(BTN_STOP, INPUT_PULLUP);
        pinMode(MOTOR, OUTPUT);

        // Attach interrupts
        attachInterrupt(
            digitalPinToInterrupt(BTN_STOP), 
            stopMachine, FALLING);
        attachInterrupt(
            digitalPinToInterrupt(BTN_START), 
            startMachine, FALLING);

        // Turn off LEDs
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, HIGH);
        delay(1800);

        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
    }
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("No internet connection...");
        delay(1000);
        return;
    }

    // Retrieve sensor data
    sensor = http.retrieveSensors();
    delay(10);

    // Check if the machine is initialized and started
    if (sensor.is_initialized && sensor.is_start) {
        digitalWrite(LED_GREEN, HIGH);
        
        // Run machine while the state is "START"
        while (state.equals(START)) {
            Serial.flush();

            // Get motor speed from potentiometer
            int value = map(analogRead(POTENTIOMETER), 0, 4095, 0, 255);  
            analogWrite(MOTOR, value);

            // Read temperature
            sensor.temperature = thermo.readCelsius();
            delay(10);

            // Update sensor values
            http.updateSensors(sensor);       

            // Check if machine should stop
            if (sensor.is_stop) {
                state = STOP;
                break;
            }

            // Read sensor data
            sensor = http.retrieveSensors();

            // Check if machine should stop
            if (sensor.is_stop) {
                state = STOP;
                break;
            }

            delay(10);
        }

        // Check if machine is not stopped prematurely
        if (!sensor.is_stop) return;

    } else {
        return;
    }
        
    // Stop the machine
    if (state.equals(STOP) || sensor.is_stop) {
        analogWrite(MOTOR, 0);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        delay(1800);
        digitalWrite(LED_RED, LOW);

        // Reset sensor values
        state = NONE; 
        sensor.is_start = false;
        sensor.is_initialized = false;
        sensor.counter = 0;
        sensor.timer = 0;
        sensor.temperature = 0;
        sensor.is_stop = false;
        http.updateSensors(sensor);
    }
}

// Interrupt service routine for stop button
void stopMachine() {
    if (state.equals(START)) {
        sensor.is_stop = true;
        Serial.println("Stop button pressed.");
    }
}

// Interrupt service routine for start button
void startMachine() {
    if (state.equals(NONE) && sensor.is_start && sensor.is_initialized) {
        state = START;
        sensor.is_stop = false;
        Serial.println("Start button pressed.");
    }
}
