#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class httpclient
{
private:
public:
    httpclient(char *server_url);
    ~httpclient();
};
