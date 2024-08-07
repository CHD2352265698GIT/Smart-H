#include <MQTT.h>

Connect_Emqx *set;
// MQTT消息回调函数，该函数会在PubSubClient对象的loop方法中被调用
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    // Serial.printf("Message arrived in topic: %s, length %d\n", topic, length);
    // Serial.print("Message:");
    // for (unsigned int i = 0; i < length; i++)
    // {
    //     Serial.print((char)payload[i]);
    // }
    set->LED_status = payload[0] - '0';
    Serial.printf("LED status: %d", (int)set->LED_status);
    set->setLed(set->LED_status); // 设置LED状态
    Serial.println("\n----------------END----------------");
}
WiFiClient Client;
PubSubClient PubSub;
Connect_aliyun::Connect_aliyun()
{
    espClient = &Client;                    // 创建网络连接客户端
    mqttClient = &PubSub;                   // 通过网络客户端连接创建mqtt连接客户端
    mqttClient->setClient(Client);          // 设置mqtt连接客户端的网络客户端
    mqttClient->setCallback(mqtt_callback); // 设置mqtt连接客户端的回调函数
    clientReconnect();                      // 连接mqtt服务器
}

void Connect_aliyun::clientReconnect() // 重连函数, 如果客户端断线,可以通过此函数重连
{
    while (mqttClient->connected() == false) // 连接mqtt服务器
    {
        Serial.printf("connected %s MQTT\n", name);
        if (connectAliyunMQTT(*mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("连接成功");
            mqttSubscribe(); // 订阅主题
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

void Connect_aliyun::mqttPublish(const char *Message) // mqtt发布post消息(上传数据)
{
    if (mqttClient->connected())
    {
        // 先拼接出json字符串
        char jsonBuf[128];
        postMsgId += 1;
        sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, Message);
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

void Connect_aliyun::mqttSubscribe()
{
    if (PubSub.connected())
    {
        PubSub.subscribe(ALINK_TOPIC_PROP_SET); // 订阅设备下发的命令
        Serial.println("订阅ALINK_TOPIC_PROP_SET");
        PubSub.subscribe(ALINK_TOPIC_PROP_POST); // 订阅设备下发的命令
        Serial.println("订阅ALINK_TOPIC_PROP_POST");
        PubSub.subscribe(ALINK_TOPIC_PROP_CONTROL); // 订阅用户下发的信息
        Serial.println("订阅ALINK_TOPIC_PROP_CONTROL");
    }
}

Connect_Emqx::Connect_Emqx()
{
    set = this;
    PubSub.setClient(Client);
    PubSub.setServer(MQTT_SERVER, MQTT_PORT);
    PubSub.setCallback(mqtt_callback);
    PubSub.setKeepAlive(10);
    clientReconnect();
}
void Connect_Emqx::clientReconnect()
{
    while (!PubSub.connected())
    {
        Serial.printf("connected %s MQTT\n", name);
        if (PubSub.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS))
        {
            Serial.println("连接成功");
            mqttSubscribe(); // 订阅主题
        }
        else
        {
            Serial.println("failed");
            Serial.println(PubSub.state());
            Serial.println("try again in 5 sec");
            for (int i = 0; i < 10; i++)
            {
                delay(500);
                Serial.println('.');
            }
        }
    }
}
void Connect_Emqx::mqttSubscribe()
{
    if (PubSub.connected())
    {
        PubSub.subscribe(MQTT_TOPIC); // 订阅设备下发的命令
        Serial.println("订阅MQTT_TOPIC");
    }
}
void Connect_Emqx::mqttPublish(const char *Message) // mqtt发布post消息(上传数据)
{
    if (PubSub.publish(MQTT_TOPIC, Message))
    {
        Serial.print("Post message to cloud: ");
        Serial.println(Message);
    }
    else
    {
        Serial.println("Publish message to cloud failed!");
    }
}

PubSubClient *Connect_Emqx::getMQTTClient()
{
    return &PubSub;
}

WiFiClient *Connect_Emqx::getWiFiClient()
{
    return &Client;
}
