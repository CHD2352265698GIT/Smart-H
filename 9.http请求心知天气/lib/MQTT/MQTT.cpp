#include <MQTT.h>

// 这是post上传数据使用的模板
const char *ALINK_BODY_FORMAT = "{\"id\":\"%u\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"method\":\"%s\",\"params\":%s}";
// 设备下发命令的set主题
const char *ALINK_TOPIC_PROP_SET = "/sys/k17hwEK9BNY/smart-H/thing/service/property/set";
// 设备上传数据的post主题
const char *ALINK_TOPIC_PROP_POST = "/sys/k17hwEK9BNY/smart-H/thing/event/property/post";
// 设备post上传数据要用到一个json字符串, 这个是拼接postJson用到的一个字符串
const char *ALINK_METHOD_PROP_POST = "thing.event.property.post";
// 订阅用户下发的信息
const char *ALINK_TOPIC_PROP_CONTROL = "/k17hwEK9BNY/smart-H/user/get";

// MQTT消息回调函数，该函数会在PubSubClient对象的loop方法中被调用
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("Message arrived in topic: %s, length %d\n", topic, length);
    Serial.print("Message:");
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println("\n----------------END----------------");
}
WiFiClient Client;
PubSubClient PubSub;
MQTT::MQTT()
{
    espClient = &Client;                    // 创建网络连接客户端
    mqttClient = &PubSub;                   // 通过网络客户端连接创建mqtt连接客户端
    mqttClient->setClient(Client);          // 设置mqtt连接客户端的网络客户端
    mqttClient->setCallback(mqtt_callback); // 设置mqtt连接客户端的回调函数
    clientReconnect();                      // 连接mqtt服务器
}
void MQTT::mqttSubscribe()
{
    if (mqttClient->connected())
    {
        // mqttClient->subscribe(ALINK_TOPIC_PROP_SET);     // 订阅设备下发的命令
        // mqttClient->subscribe(ALINK_TOPIC_PROP_POST); // 订阅设备下发的命令
        // mqttClient->subscribe(ALINK_TOPIC_PROP_CONTROL); // 订阅用户下发的信息
    }
}
void MQTT::clientReconnect() // 重连函数, 如果客户端断线,可以通过此函数重连
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

void MQTT::mqttPublish(const char *Message) // mqtt发布post消息(上传数据)
{
    if (mqttClient->connected())
    {
        // 先拼接出json字符串
        char param[32];
        char jsonBuf[128];
        sprintf(param, Message); // 把要上传的数据写在param里
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