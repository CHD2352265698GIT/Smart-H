#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <LED.h>
#include <WIFI_STA_AP.h>
#include <STEER_MOTOR.h>
#include <PubSubClient.h>
#include <MQTT.h>
#include <Ticker.h>

WIFI_STA_AP WIFI; // 实例化WIFI_STA_AP类
MQTT *aliyu;
const char *Message = "{\"LEDSwitch\":1}";
void WifiConnectCallBack()
{
  aliyu = new MQTT;
  aliyu->clientReconnect();    // 连接mqtt服务器
  aliyu->mqttPublish(Message); // 发布消息
  digitalWrite(LED_PIN, HIGH); // 关闭LED
}

void setup()
{
  p_WIFI_STA_AP = &WIFI;                          // 保存WIFI_STA_AP类的指针
  Serial.begin(115200);                           // 设置串口波特率
  WIFI.WifiConnectCallBack = WifiConnectCallBack; // 设置WIFI连接回调函数
  WIFI.initSoftAP_STA();                          // 初始化WIFI SoftAP模式
  WIFI.initWebServer();                           // 初始化WIFI WebServer
  pinMode(LED_PIN, OUTPUT);                       // 设置GPIO模式为输出
  WIFI.connectNewWifi();                          // 连接WIFI
}

void loop()
{
  esp8266_server.handleClient(); // 处理http服务器访问
}
