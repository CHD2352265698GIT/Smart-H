#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <LED.h>
#include <WIFI_STA_AP.h>
#include <STEER_MOTOR.h>
#include <PubSubClient.h>
#include <MQTT.h>
#include <Ticker.h>
WIFI_STA_AP WIFI;                   // 实例化WIFI_STA_AP类
STEER_MOTOR Steer(STEER_MOTOR_PIN); // 实例化STEER_MOTOR类
MQTT *aliyu;
Ticker tim1; // 这个定时器是为了每5秒上传一次数据

void test()
{
  aliyu->mqttPublish(); // 发布消息
}
void test1(char *topic, uint8_t *payload, unsigned int length)
{
  aliyu->callback(topic, payload, length); // 处理mqtt消息
}
void setup()
{
  p_WIFI_STA_AP = &WIFI;                      // 保存WIFI_STA_AP类的指针
  Serial.begin(115200);                       // 设置串口波特率
  WIFI.initSoftAP_STA();                      // 初始化WIFI SoftAP模式
  WIFI.initWebServer();                       // 初始化WIFI WebServer
  pinMode(LED_PIN, OUTPUT);                   // 设置GPIO模式为输出
  WIFI.connectNewWifi();                      // 连接到新的WIFI网络
  Steer.turnToAngle(30, 'N');                 // 舵机转动到30度
  delay(1000);                                // 延时1秒
  Steer.turnToAngle(120);                     // 舵机转动到30度
  aliyu = new MQTT;                           // 实例化MQTT类
  aliyu->getMQTTClient()->setCallback(test1); // 绑定收到set主题时的回调(命令下发回调)
  tim1.attach(5, test);                       // 启动每5秒发布一次消息
}

void loop()
{
  esp8266_server.handleClient();            // 处理http服务器访问
  if (!aliyu->getMQTTClient()->connected()) // 再看mqtt连接了没
  {
    Serial.println("mqtt disconnected!Try reconnect now...");
    Serial.println(aliyu->getMQTTClient()->state());
    aliyu->clientReconnect();
  }
  Steer.service();                // 控制舵机
  aliyu->getMQTTClient()->loop(); // mqtt客户端监听
}
