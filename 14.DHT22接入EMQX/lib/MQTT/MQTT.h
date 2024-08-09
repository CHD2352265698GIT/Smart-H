#pragma once
#include <string>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aliyun_mqtt.h>
#include <ArduinoJson.h>
#include <STEER_MOTOR.h>
class Connect_Emqx : public STEER_MOTOR
{
public:
    Connect_Emqx();                                           // 连接MQTT服务器
    void clientReconnect();                                   // 重连MQTT服务器
    void mqttPublish(const char *topic, const char *Message); // mqtt发布post消息(上传数据)
    void mqttSubscribe();                                     // mqtt订阅post消息(上传数据)
    PubSubClient *getMQTTClient();                            // 获取MQTT客户端
    WiFiClient *getWiFiClient();                              // 获取WiFi客户端
    bool LED_status;                                          // 灯的状态
    const char *MQTT_TOPIC_LED = "LED_Switch";                // MQTT主题
    const char *MQTT_TOPIC_Temp = "DHT_Temp";                 // MQTT主题
    const char *MQTT_TOPIC_Hum = "DHT_Hum";                   // MQTT主题
    const char *MQTT_TOPIC_DHT_STATE = "DHT_state";           // MQTT主题
private:
    const char *name = "emqx";                       // MQTT服务器名称
    const char *MQTT_SERVER = "114.132.176.197";     // MQTT服务器地址
    const int MQTT_PORT = 1883;                      // MQTT服务器端口
    const char *MQTT_USER = "admin";                 // MQTT用户名
    const char *MQTT_PASS = "asdfghjkl@123ABC";      // MQTT密码
    const char *MQTT_CLIENT_ID = "Bedroom_Light";    // MQTT客户端ID
    const char *MQTT_TOPIC_SUB_LED = MQTT_TOPIC_LED; // MQTT订阅主题
};
class Connect_aliyun
{
public:
    Connect_aliyun();                      // 连接阿里云MQTT服务器
    ~Connect_aliyun();                     // 断开阿里云MQTT服务器
    void clientReconnect();                // 重连阿里云MQTT服务器
    void mqttPublish(const char *Message); // mqtt发布post消息(上传数据)
    void mqttSubscribe();                  // mqtt订阅post消息(上传数据)
    PubSubClient *getMQTTClient();         // 获取MQTT客户端
    WiFiClient *getWiFiClient();           // 获取WiFi客户端
private:
    WiFiClient *espClient;                                                                    // WiFi客户端
    PubSubClient *mqttClient;                                                                 // MQTT客户端
    const char *name = "aliyun";                                                              // MQTT服务器名称
    const char *PRODUCT_KEY = "k17hwEK9BNY";                                                  // 产品ID
    const char *DEVICE_NAME = "smart-H";                                                      // 设备名
    const char *DEVICE_SECRET = "9f4d607789c158825f51e92c49055c9e";                           // 设备key
    const char *ALINK_BODY_FORMAT = "{\"id\":\"%u\",\"version\":\"1.0\",\
    \"sys\":{\"ack\":0},\"method\":\"%s\",\"params\":%s}";                                    // post上传数据使用的模板
    const char *ALINK_TOPIC_PROP_SET = "/sys/k17hwEK9BNY/smart-H/thing/service/property/set"; // 设备下发命令的set主题
    const char *ALINK_TOPIC_PROP_POST = "/sys/k17hwEK9BNY/smart-H/thing/event/property/post"; // 设备上传数据的post主题
    const char *ALINK_METHOD_PROP_POST = "thing.event.property.post";                         // 设备post上传数据要用到一个json字符串, 这个是拼接postJson用到的一个字符串
    const char *ALINK_TOPIC_PROP_CONTROL = "/k17hwEK9BNY/smart-H/user/get";                   // 订阅用户下发的信息
    int postMsgId = 0;                                                                        // 上传数据的消息id, 用于区分不同的上传数据
};
