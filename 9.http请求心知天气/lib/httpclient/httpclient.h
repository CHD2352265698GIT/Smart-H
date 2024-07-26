#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class httpclientData
{
public:
    int weather_code;
    int temperature;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    String weather_txt;
};
class weather
{
private:
    char *server_url;
    WiFiClient *espClient;
    void AnalysisJson(const char *json, httpclientData &result);

public:
    weather(WiFiClient *espClient);
    void weatherRequest(const char *key, const char *Request_location, httpclientData &result);
};

class httptime
{
public:
    httptime(WiFiClient *espClien, httpclientData &result);
};