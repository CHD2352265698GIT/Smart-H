#pragma once

#include <string>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MQTT
{
public:
    MQTT(const char *clientID, const char *server, uint16_t port);
    void begin();
};