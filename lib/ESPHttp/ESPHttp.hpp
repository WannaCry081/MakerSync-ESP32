#ifndef ESPHTTP_HPP
#define ESPHTTP_HPP


#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


typedef struct {
    boolean is_start; 
    boolean is_stop;
    boolean is_initialized;
    int counter;
    int timer; 
    float temperature; 
} Sensor;


class ESPHttp {
    private:
        const String _sensors_url = "https://maker-sync-django.vercel.app/api/v1/sensors/";
        const String _users_url = "https://maker-sync-django.vercel.app/api/v1/users/";

    public:
        Sensor retrieveSensors(String sensor_id);     
        void createSensors(String sensor_id);
        void updateSensors(String sensor_id, Sensor sensor);  
};


#endif