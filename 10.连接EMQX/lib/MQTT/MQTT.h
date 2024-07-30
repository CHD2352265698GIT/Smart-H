#pragma once

#include <string>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aliyun_mqtt.h>
#include <ArduinoJson.h>

#define LED_B 2 // 定义LED灯的引脚
extern const char *Message;
extern void mqtt_callback(char *topic, byte *payload, unsigned int length); // 回调函数
extern WiFiClient Client;

class Connect_Emqx
{
public:
    Connect_Emqx();
    void clientReconnect();
    void mqttPublish(const char *Message); // mqtt发布post消息(上传数据)
    void mqttSubscribe();                  // mqtt订阅post消息(上传数据)
    PubSubClient *getMQTTClient();
    WiFiClient *getWiFiClient();

private:
    const char *name = "emqx";
    const char *MQTT_SERVER = "114.132.176.197";
    const int MQTT_PORT = 1883;
    const char *MQTT_USER = "admin";
    const char *MQTT_PASS = "asdfghjkl@123ABC";
    const char *MQTT_CLIENT_ID = "esp8266";
    const char *MQTT_TOPIC = "esp8266/led";
    const char *MQTT_TOPIC_SUB = MQTT_TOPIC;
};
class Connect_aliyun
{
public:
    Connect_aliyun();
    ~Connect_aliyun();
    void clientReconnect();
    void mqttPublish(const char *Message); // mqtt发布post消息(上传数据)
    void mqttSubscribe();                  // mqtt订阅post消息(上传数据)
    PubSubClient *getMQTTClient()
    {
        return mqttClient;
    }
    WiFiClient *getWiFiClient()
    {
        return espClient;
    }

private:
    WiFiClient *espClient;
    PubSubClient *mqttClient;
    const char *name = "aliyun";
    const char *PRODUCT_KEY = "k17hwEK9BNY";                        // 产品ID
    const char *DEVICE_NAME = "smart-H";                            // 设备名
    const char *DEVICE_SECRET = "9f4d607789c158825f51e92c49055c9e"; // 设备key
    // post上传数据使用的模板
    const char *ALINK_BODY_FORMAT = "{\"id\":\"%u\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"method\":\"%s\",\"params\":%s}";
    // 设备下发命令的set主题
    const char *ALINK_TOPIC_PROP_SET = "/sys/k17hwEK9BNY/smart-H/thing/service/property/set";
    // 设备上传数据的post主题
    const char *ALINK_TOPIC_PROP_POST = "/sys/k17hwEK9BNY/smart-H/thing/event/property/post";
    // 设备post上传数据要用到一个json字符串, 这个是拼接postJson用到的一个字符串
    const char *ALINK_METHOD_PROP_POST = "thing.event.property.post";
    // 订阅用户下发的信息
    const char *ALINK_TOPIC_PROP_CONTROL = "/k17hwEK9BNY/smart-H/user/get";
    int postMsgId = 0; // 上传数据的消息id, 用于区分不同的上传数据
};
