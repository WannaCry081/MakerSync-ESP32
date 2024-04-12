#include "ESPHttp.hpp"


ESPHttp::ESPHttp(const String & uuid) {
    _uuid = uuid;
}   


Sensor ESPHttp::retrieveSensors() {
    Sensor sensor;
    HTTPClient http;

    String endpoint = _sensors_url + sensor_id;
    http.begin(endpoint);

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


bool ESPHttp::createSensors(String sensor_id) {
    HTTPClient http;

    String endpoint = _sensors_url + sensor_id;
    http.addHeader("Content-Type", "application/json");

    http.begin(endpoint);

    int httpResponseCode = http.POST("");

    http.end();

    return (httpResponseCode == 201) ? true : false;
}


bool ESPHttp::updateSensors(String sensor_id, Sensor sensor) {
    HTTPClient http;

    String endpoint = _sensors_url + sensor_id;

    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"is_start\":\"" + String(sensor.is_start) + "\", ";
    payload += "\"is_stop\":\"" + String(sensor.is_stop) + "\", ";
    payload += "\"is_initialized\":\"" + String(sensor.is_initialized) + "\", ";
    payload += "\"counter\":\"" + String(sensor.counter) + "\", ";
    payload += "\"timer\":\"" + String(sensor.timer) + "\", ";
    payload += "\"temperature\":\"" + String(sensor.temperature) + "\"";
    payload += "}";

    http.begin(endpoint);
    int httpResponseCode = http.PUT(payload);

    http.end();
    return (httpResponseCode == 200) ? true : false;
}  