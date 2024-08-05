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

void setup()
{
  p_WIFI_STA_AP = &WIFI;        // 保存WIFI_STA_AP类的指针
  Serial.begin(115200);         // 设置串口波特率
  myservo.attach(2, 500, 2500); // 修正脉冲宽度
  myservo.write(20);
  delay(1000);
  // 停止脉冲
  myservo.detach();

  WIFI.initSoftAP_STA();    // 初始化WIFI SoftAP模式
  WIFI.initWebServer();     // 初始化WIFI WebServer
  pinMode(LED_PIN, OUTPUT); // 设置GPIO模式为输出
  // WIFI.Read_WIFI_STA_AP_Config();     // 读取flash保存的wifi信息
  // WiFi.begin(sta_ssid, sta_password); // 连接上一次连接成功的wifi
  digitalWrite(LED_PIN, HIGH); // 关闭LED
}

void loop()
{
  esp8266_server.handleClient(); // 处理http服务器访问
}
