#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


#include <Arduino.h>

// Machine UUID
const String UUID = "2ae09d7a-8f5d-496f-9917-4deabb3e5ad3";

// Wifi credentials
const String WIFI_SSID = "WIFI_SSID_HERE";
const String WIFI_PASSWORD = "WIFI_PASSWORD_HERE";

// LED pins
const byte LED_RED = 14;
const byte LED_YELLOW = 12; 
const byte LED_GREEN = 13;

// Button pins
const byte BTN_START = 18;
const byte BTN_STOP = 19;

// Thermocouple pin
const byte SCK_PIN = 27;
const byte CS_PIN = 26;
const byte SO_PIN = 25;

// Relay module pin
const byte RELAY_MODULE = 33;

#endif