#include "ESPHttp.hpp"


ESPHttp::ESPHttp(const String & uuid) {
    _uuid = uuid;
    _sensorsUrl = _machinesUrl + "/" + uuid + "/sensors";
    _notificationsUrl = _machinesUrl + "/" + uuid + "/notifications";
}   

bool ESPHttp::createMachine(void) {

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

Sensor ESPHttp::createSensors(void) {
    Sensor sensor;
    HTTPClient http;

    http.addHeader("Content-Type", "application/json");
    http.begin(_sensorsUrl);

    int httpCode = http.POST("");
    
    if (httpCode == 201) {
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

bool ESPHttp::updateSensors(Sensor sensor) {
    HTTPClient http;

    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"is_start\":\"" + String(sensor.is_start) + "\", ";
    payload += "\"is_stop\":\"" + String(sensor.is_stop) + "\", ";
    payload += "\"is_initialize\":\"" + String(sensor.is_initialize) + "\", ";
    payload += "\"counter\":\"" + String(sensor.counter) + "\", ";
    payload += "\"time\":\"" + String(sensor.time) + "\", ";
    payload += "\"temperature\":\"" + String(sensor.temperature) + "\"";
    payload += "}";

    http.begin(_sensorsUrl);
    int httpCode = http.PUT(payload);

    http.end();
    return (httpCode == 200) ? true : false;
}

void ESPHttp::createNotifications(String title, String content) {
    HTTPClient http;
    Serial.println(_notificationsUrl);

    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"title\":\"" + title + "\", ";
    payload += "\"content\":\"" + content + "\" ";
    payload += "}";

    http.begin(_notificationsUrl);
    http.POST(payload);
    http.end();
}