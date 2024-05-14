#ifndef ESPHTTP_HPP
#define ESPHTTP_HPP


#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


typedef struct {
    bool is_start; 
    bool is_stop;
    bool is_initialize;
    int counter;
    int time; 
    float temperature; 
} Sensor;


class ESPHttp {
    private:
        String _uuid;
        
        const String _machinesUrl = "https://maker-sync-django.vercel.app/api/v2/machines";
        const String _sensorsUrl = _machinesUrl + _uuid + "/sensors";
        const String _notificationsUrl = _machinesUrl + _uuid + "/notifications";

    public:
        ESPHttp(const String & uuid);
        Sensor retrieveSensors();     
        bool createSensors();
        bool updateSensors(Sensor sensor);  
};


#endif