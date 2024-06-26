#ifndef ESPWIFI_HPP
#define ESPWIFI_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <Update.h>
#include <LittleFS.h>
#include <SD.h>


class ESPWifi {
    private:
        int _counter = 0;
        
    public:
        ESPWifi(const String& ssid, const String& password);
        bool isConnect();
        bool isSuccess();
        void displayStatus();
};


#endif