#pragma once

#include <string>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aliyun_mqtt.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#define PRODUCT_KEY "k17hw7wWjkx"                        // 产品ID
#define DEVICE_NAME "bedroom_lamp"                       // 设备名
#define DEVICE_SECRET "bc1e65f664c53a79e9d6930758111ef3" // 设备key
#define LED_B 2                                          // 定义LED灯的引脚
class MQTT
{
public:
    MQTT()
    {
        espClient = new WiFiClient;        // 创建网络连接客户端
        mqttClient = new PubSubClient;     // 通过网络客户端连接创建mqtt连接客户端
        mqttClient->setClient(*espClient); // 设置mqtt连接客户端的网络客户端
        clientReconnect();                 // 连接mqtt服务器
    }
    ~MQTT()
    {
        // delete mqttClient;
        delete espClient;
    }

    void clientReconnect() // 重连函数, 如果客户端断线,可以通过此函数重连
    {
        while (mqttClient->connected() == false) // 连接mqtt服务器
        {
            Serial.println("connected Aliyun MQTT");
            if (connectAliyunMQTT(*mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
            {
                Serial.println("连接成功");
            }
            else
            {
                Serial.println("failed");
                Serial.println(mqttClient->state());
                Serial.println("try again in 5 sec");
                for (int i = 0; i < 10; i++)
                {
                    delay(500);
                    Serial.println('.');
                }
            }
        }
    }
    void mqttPublish() // mqtt发布post消息(上传数据)
    {
        if (mqttClient->connected())
        {
            // 先拼接出json字符串
            char param[32];
            char jsonBuf[128];
            sprintf(param, "{\"LED_Switch\":%d}", digitalRead(LED_B)); // 把要上传的数据写在param里
            postMsgId += 1;
            sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, param);
            // 再从mqtt客户端中发布post消息
            if (mqttClient->publish(ALINK_TOPIC_PROP_POST, jsonBuf))
            {
                Serial.print("Post message to cloud: ");
                Serial.println(jsonBuf);
            }
            else
            {
                Serial.println("Publish message to cloud failed!");
            }
        }
    }
    // 收到set主题的命令下发时的回调函数,(接收命令)
    void callback(char *topic, byte *payload, unsigned int length)
    {
        // if (strstr(topic, ALINK_TOPIC_PROP_SET))
        // // 如果收到的主题里包含字符串ALINK_TOPIC_PROP_SET(也就是收到/sys/a17lGhkKwXs/esp32LightHome/thing/service/property/set主题)
        // {
        //     Serial.println("收到下发的命令主题:");
        //     Serial.println(topic);
        //     Serial.println("下发的内容是:");
        //     payload[length] = '\0'; // 为payload添加一个结束附,防止Serial.println()读过了
        //     Serial.println((char *)payload);

        //     // 接下来是收到的json字符串的解析
        //     JsonDocument doc(100);
        //     DeserializationError error = deserializeJson(doc, payload);
        //     if (error)
        //     {
        //         Serial.println("parse json failed");
        //         return;
        //     }
        //     JsonObject setAlinkMsgObj = doc.as<JsonObject>();
        //     serializeJsonPretty(setAlinkMsgObj, Serial);
        //     Serial.println();

        //     // 这里是一个点灯小逻辑
        //     int lightSwitch = setAlinkMsgObj["params"]["LightSwitch"];
        //     digitalWrite(LED_B, lightSwitch);
        //     mqttPublish(); // 由于将来做应用可能要获取灯的状态,所以在这里发布一下
        // }
    }
    PubSubClient *getMQTTClient()
    {
        return mqttClient;
    }

private:
    WiFiClient *espClient;
    PubSubClient *mqttClient;
    int postMsgId = 0; // 上传数据的消息id, 用于区分不同的上传数据
    // 这是post上传数据使用的模板
    const char *ALINK_BODY_FORMAT = "{\"id\":\"%u\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}";
    // 设备下发命令的set主题
    const char *ALINK_TOPIC_PROP_SET = "/sys/k17hw7wWjkx/bedroom_lamp/thing/service/property/set";
    // 设备上传数据的post主题
    const char *ALINK_TOPIC_PROP_POST = "/sys/k17hw7wWjkx/bedroom_lamp/thing/event/property/post";
    // 设备post上传数据要用到一个json字符串, 这个是拼接postJson用到的一个字符串
    const char *ALINK_METHOD_PROP_POST = "thing.event.property.post";
};