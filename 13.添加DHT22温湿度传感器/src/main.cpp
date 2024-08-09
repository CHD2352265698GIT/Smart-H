#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <LED.h>
#include <WIFI_STA_AP.h>
#include <STEER_MOTOR.h>
#include <MQTT.h>
#include <Timer_Task.h>
#include <Ticker.h>
#include <httpclient.h>
#include <LittleFS.h>
#include <DHT22.h>
using namespace std;
const char *Message = "{\"LEDSwitch\":%d}";   // 发送消息格式
const char *Weatherkey = "SDpJpH-c8vI4OaOYJ"; // 心知天气key
WIFI_STA_AP WIFI;                             // 实例化WIFI_STA_AP类
Connect_Emqx *Emqx;                           // 实例化Connect_Emqx类
Ticker timer1;                                // 实例化Ticker类
Timer_Task Task1(1);                          // 实例化Timer_Task类
Timer_Task Task2(10);                         // 实例化Timer_Task类
Timer_Task Task3(5);                          // 实例化Timer_Task类
httpclientData http_data;                     // 实例化httpclientData类
void WifiConnectCallBack()                    // WIFI连接成功回调函数
{
  Emqx = new Connect_Emqx;                                                 // 实例化Connect_Emqx类
  char motorvalue[2];                                                      // 临时保存舵机角度
  spi_flash_read(WIFI_MOTOR_ANGLE_ADDR, (uint32_t *)motorvalue, 2);        // 读取舵机角度
  Emqx->setOnValue((int)motorvalue[0]);                                    // 设置mqtt消息中的舵机角度
  Emqx->setOffValue((int)motorvalue[1]);                                   // 设置mqtt消息中的舵机角度
  Serial.printf("On:%d,Off:%d\n", (int)motorvalue[0], (int)motorvalue[1]); // 打印舵机角度
  Emqx->clientReconnect();                                                 // 连接mqtt服务器
  led.blink(8);                                                            // LED闪烁，8次
  digitalWrite(LED_PIN, HIGH);                                             // 关闭LED
}

void setup()
{
  p_WIFI_STA_AP = &WIFI;                          // 保存WIFI_STA_AP类的指针
  Serial.begin(921600);                           // 设置串口波特率
  WIFI.WifiConnectCallBack = WifiConnectCallBack; // 设置WIFI连接回调函数
  WIFI.initSoftAP_STA();                          // 初始化WIFI SoftAP模式
  WIFI.initWebServer();                           // 初始化WIFI WebServer
  pinMode(LED_PIN, OUTPUT);                       // 设置GPIO模式为输出
  WIFI.connectNewWifi();                          // 连接WIFI
  timer1.attach(1, []()
                {Task1.Run();Task2.Run(); 
                Task3.Run(); }); // 定时器1，中断时间为1秒
}

void loop()
{
  // mqtt客户端监听
  Emqx->getMQTTClient()->loop();
  // Task1任务，每秒打印一次时间
  Task1.RunTask([]()
                { Serial.println(millis() / 1000); });
  // Task2任务，每10秒执行一次
  Task2.RunTask([]()
                {
                  if (!Emqx->getMQTTClient()->connected()) // 检查mqtt是否断开连接
                    {
                      Serial.println("mqtt disconnected!Try reconnect now..."); // 打印mqtt断开连接
                      Serial.println(Emqx->getMQTTClient()->state());           // 打印mqtt连接状态
                      Emqx->clientReconnect();                                  // 重新连接mqtt服务器
                    } });
  // Task3任务，每5秒执行一次
  Task3.RunTask([]()
                {
                  DHT22 *dht = new DHT22;                                                                 // 实例化DHT22类
                  Serial.printf("引脚:%d  ", dht->pin);                                                   // 打印引脚号
                  if (!dht->readData())                                                                   // 读取温湿度数据
                    Serial.printf("温度:%.1f℃ 湿度:%.1f%%\n", dht->getTemperature(), dht->getHumidity()); // 打印温湿度数据
                  else
                  {
                    Serial.println(dht->ErrorData()); // 打印错误信息
                  }
                  delete dht; // 释放内存
                });
}
