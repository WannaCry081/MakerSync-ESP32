#include "ESPHttp.hpp"


Sensor ESPHttp::retrieveSensors(String sensor_id) {
    Sensor sensor;
    HTTPClient http;

    String url = _sensors_url + sensor_id;
    http.begin(url);

    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();

        JsonDocument doc;
        deserializeJson(doc, payload);

        sensor.is_start = (doc["is_start"].as<JsonInteger>() != 0);
        sensor.is_stop = (doc["is_stop"].as<JsonInteger>() != 0);
        sensor.is_initialized = (doc["is_initialized"].as<JsonInteger>() != 0);
        sensor.counter = doc["counter"].as<JsonInteger>();
        sensor.timer = doc["timer"].as<JsonInteger>();;
        sensor.temperature = doc["temperature"].as<JsonFloat>();
    }

    http.end();
    return sensor;
}   


void ESPHttp::createSensors(String sensor_id) {

}


void ESPHttp::updateSensors(String sensor_id, Sensor sensor) {

}  