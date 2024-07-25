#pragma once

#include <string>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aliyun_mqtt.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#define PRODUCT_KEY "k17hwEK9BNY"                        // 产品ID
#define DEVICE_NAME "smart-H"                            // 设备名
#define DEVICE_SECRET "9f4d607789c158825f51e92c49055c9e" // 设备key
#define LED_B 2                                          // 定义LED灯的引脚
extern const char *Message;
extern void mqtt_callback(char *topic, byte *payload, unsigned int length); // 回调函数

class MQTT
{
public:
    MQTT();
    ~MQTT();
    void clientReconnect();
    void mqttPublish(const char *Message); // mqtt发布post消息(上传数据)
    void mqttSubscribe();                  // mqtt订阅post消息(上传数据)
    PubSubClient *getMQTTClient()
    {
        return mqttClient;
    }

private:
    WiFiClient *espClient;
    PubSubClient *mqttClient;
    int postMsgId = 0; // 上传数据的消息id, 用于区分不同的上传数据
};