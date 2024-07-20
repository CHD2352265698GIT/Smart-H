#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <LED.h>
#include <Webserver.h>
#include <WIFI_STA_AP.h>

Webserver webserver; // 实例化Webserver类
WIFI_STA_AP WIFI;    // 实例化WIFI_STA_AP类

void setup()
{
  p_WIFI_STA_AP = &WIFI; // 保存WIFI_STA_AP类的指针
  Serial.begin(115200); // 设置串口波特率

  pinMode(LED_PIN, OUTPUT); // 设置GPIO模式为输出
  WIFI.connectNewWifi();    // 连接到新的WIFI网络
}

void loop()
{
  esp8266_server.handleClient();  // 处理http服务器访问
  dnsServer.processNextRequest(); // 处理DNS请求
}
