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
        String _sensorsUrl;
        String _notificationsUrl;
        
    public:
        ESPHttp(const String & uuid);

        bool createMachine(void);
        
        Sensor createSensors(void);
        Sensor retrieveSensors(void);     
        bool updateSensors(Sensor sensor);  

        void createNotifications(String title, String content);
};


#endif