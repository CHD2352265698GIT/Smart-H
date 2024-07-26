#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class weather_Data
{
public:
    int weather_code;
    int temperature;
    String weather_txt;
};
class weather
{
private:
    char *server_url;
    WiFiClient *espClient;
    void AnalysisJson(const char *json, weather_Data &result);

public:
    weather(WiFiClient *espClient);
    void weatherRequest(const char *key, const char *Request_location, weather_Data &result);
};