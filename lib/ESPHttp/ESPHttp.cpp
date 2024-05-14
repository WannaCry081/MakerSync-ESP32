#include "ESPHttp.hpp"


ESPHttp::ESPHttp(const String & uuid) {
    _uuid = uuid;
}   

bool ESPHttp::createMachine(void) {

Sensor ESPHttp::retrieveSensors() {
    Sensor sensor;
    HTTPClient http;

    http.addHeader("Content-Type", "application/json");
    http.begin(_machinesUrl);

    String payload = "{code : " + _uuid + "}";
    int httpCode = http.POST(payload);
    

    http.end();
    return (httpCode == 201) ? true : false;
}

Sensor ESPHttp::retrieveSensors() {
    Sensor sensor;
    HTTPClient http;

    http.begin(_sensorsUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
        String response = http.getString();

        JsonDocument doc;
        deserializeJson(doc, response);

        sensor.is_start = (doc["is_start"].as<JsonInteger>() != 0);
        sensor.is_stop = (doc["is_stop"].as<JsonInteger>() != 0);
        sensor.is_initialize = (doc["is_initialize"].as<JsonInteger>() != 0);
        sensor.counter = doc["counter"].as<JsonInteger>();
        sensor.time = doc["time"].as<JsonInteger>();
        sensor.temperature = doc["temperature"].as<JsonFloat>();
    }

    http.end();
    return sensor;
}   


bool ESPHttp::createSensors() {
    HTTPClient http;

    String endpoint = _sensors_url + _uuid;
    http.addHeader("Content-Type", "application/json");

    http.begin(endpoint);

    int httpResponseCode = http.POST("");

    http.end();

    return (httpResponseCode == 201) ? true : false;
}


bool ESPHttp::updateSensors(Sensor sensor) {
    HTTPClient http;

    String endpoint = _sensors_url + _uuid;

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