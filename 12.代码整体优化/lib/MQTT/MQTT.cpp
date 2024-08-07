#include <MQTT.h>

Connect_Emqx *set;                                                  // 创建一个全局指针，指向Connect_Emqx类
static WiFiClient Client;                                           // 创建网络连接客户端
static PubSubClient PubSub;                                         // 创建mqtt连接客户端
void mqtt_callback(char *topic, byte *payload, unsigned int length) // MQTT消息回调函数，该函数会在PubSubClient对象的loop方法中被调用
{
    Serial.println("----------------START----------------"); // 打印开始
    set->LED_status = payload[0] - '0';                      // 获取LED状态
    Serial.printf("LED status: %d", (int)set->LED_status);   // 打印LED状态
    set->setLed(set->LED_status);                            // 设置LED状态
    Serial.println("----------------END----------------");   // 打印结束
}
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
        Serial.printf("connected %s MQTT\n", name);                                  // 打印连接信息
        if (connectAliyunMQTT(*mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET)) // 连接mqtt服务器，产品密钥、设备名称、设备密钥，通过阿里云控制台获取
        {
            Serial.println("连接成功"); // 打印连接成功
            mqttSubscribe();            // 订阅主题
        }
        else
        {
            Serial.println("failed");             // 打印连接失败
            Serial.println(mqttClient->state());  // 打印连接失败原因
            Serial.println("try again in 5 sec"); // 打印重连信息
            for (int i = 0; i < 10; i++)          // 每隔0.5秒打印一个点，表示正在重连，5秒重新连接一次服务器
            {
                delay(500);          // 延时0.5秒
                Serial.println('.'); // 打印一个点
            }
        }
    }
}
void Connect_aliyun::mqttPublish(const char *Message) // mqtt发布post消息(上传数据)
{
    if (mqttClient->connected()) // 如果mqtt客户端连接成功,则发布post消息
    {
        char jsonBuf[128]; // 先拼接出json字符串
        postMsgId += 1;
        sprintf(jsonBuf, ALINK_BODY_FORMAT, postMsgId, ALINK_METHOD_PROP_POST, Message); // 拼接json字符串
        if (mqttClient->publish(ALINK_TOPIC_PROP_POST, jsonBuf))                         // 再从mqtt客户端中发布post消息
        {
            Serial.print("Post message to cloud: "); // 打印上传数据
            Serial.println(jsonBuf);                 //  要发布的消息
        }
        else
        {
            Serial.println("Publish message to cloud failed!"); // 打印上传数据失败
        }
    }
}
void Connect_aliyun::mqttSubscribe() // mqtt订阅主题
{
    if (PubSub.connected()) // 如果mqtt客户端连接成功,则订阅主题
    {
        PubSub.subscribe(ALINK_TOPIC_PROP_SET); // 订阅设备下发的命令
        Serial.println("订阅ALINK_TOPIC_PROP_SET");
        PubSub.subscribe(ALINK_TOPIC_PROP_POST); // 订阅设备下发的命令
        Serial.println("订阅ALINK_TOPIC_PROP_POST");
        PubSub.subscribe(ALINK_TOPIC_PROP_CONTROL); // 订阅用户下发的信息
        Serial.println("订阅ALINK_TOPIC_PROP_CONTROL");
    }
}
PubSubClient *Connect_aliyun::getMQTTClient() // 获取MQTT客户端
{
    return mqttClient;
}
WiFiClient *Connect_aliyun::getWiFiClient() // 获取WiFi客户端
{
    return espClient;
}
Connect_Emqx::Connect_Emqx() //  MQTT初始化
{
    set = this;                               // 全局指针指向当前对象
    PubSub.setClient(Client);                 // 设置mqtt客户端
    PubSub.setServer(MQTT_SERVER, MQTT_PORT); // 设置mqtt服务器，IP和端口
    PubSub.setCallback(mqtt_callback);        // 设置回调函数
    PubSub.setKeepAlive(10);                  // 设置心跳时间
    clientReconnect();                        // 连接mqtt服务器
}
void Connect_Emqx::clientReconnect() // mqtt重连
{
    while (!PubSub.connected()) // 如果mqtt客户端没有连接成功,则一直重连
    {
        Serial.printf("connected %s MQTT\n", name);               // 打印连接信息
        if (PubSub.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)) // 连接mqtt服务器，ID、用户名、密码
        {
            Serial.println("连接成功"); // 打印连接成功
            mqttSubscribe();            // 订阅主题
        }
        else
        {
            Serial.println("failed");             // 打印连接失败
            Serial.println(PubSub.state());       // 打印连接失败原因
            Serial.println("try again in 5 sec"); // 打印重连信息
            for (int i = 0; i < 10; i++)          // 每隔0.5秒打印一个点，表示正在重连，5秒回重新连接一次服务器
            {
                delay(500);          // 延时0.5秒
                Serial.println('.'); // 打印一个点
            }
        }
    }
}
void Connect_Emqx::mqttSubscribe() // mqtt订阅主题
{
    if (PubSub.connected()) // 如果mqtt客户端连接成功,则订阅主题
    {
        PubSub.subscribe(MQTT_TOPIC);                   // 订阅设备下发的命令
        Serial.printf("订阅MQTT_TOPIC:%s", MQTT_TOPIC); // 打印订阅主题
    }
}
void Connect_Emqx::mqttPublish(const char *Message) // mqtt发布post消息(上传数据)
{
    if (PubSub.publish(MQTT_TOPIC, Message)) // 发布post消息(上传数据),主题、要发布的消息
    {
        Serial.print("Post message to cloud: "); // 打印上传数据
        Serial.println(Message);                 //  要发布的消息
    }
    else
    {
        Serial.println("Publish message to cloud failed!"); // 打印上传数据失败
    }
}
PubSubClient *Connect_Emqx::getMQTTClient() // 获取MQTT客户端
{
    return &PubSub;
}
WiFiClient *Connect_Emqx::getWiFiClient() // 获取WiFi客户端
{
    return &Client;
}
