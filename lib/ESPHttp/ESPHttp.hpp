#ifndef ESPHTTP_HPP
#define ESPHTTP_HPP


#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


typedef struct {
    bool is_start; 
    bool is_stop;
    bool is_initialized;
    int counter;
    int timer; 
    float temperature; 
} Sensor;


class ESPHttp {
    private:
        const String _sensors_url = "https://maker-sync-django.vercel.app/api/v1/sensors/";
        const String _users_url = "https://maker-sync-django.vercel.app/api/v1/users/";

    public:
        Sensor retrieveSensors();     
        bool createSensors();
        bool updateSensors(Sensor sensor);  
};


#endif