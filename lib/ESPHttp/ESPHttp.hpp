#ifndef ESPHTTP_HPP
#define ESPHTTP_HPP


#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


struct Sensor {
    boolean is_start; 
    boolean is_stop;
    boolean is_initialized;
    int counter;
    int timer; 
    float temperature; 
};


class ESPHttp {
    private:
        String _base_url = "https://maker-sync-django.vercel.app/api/v1/";

    public:
        Sensor retrieveSensors(String sensor_id);       
};


#endif