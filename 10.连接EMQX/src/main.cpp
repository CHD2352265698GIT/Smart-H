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
using namespace std;
const char *Message = "{\"LEDSwitch\":%d}";
const char *Weatherkey = "SDpJpH-c8vI4OaOYJ"; // 心知天气key
WIFI_STA_AP WIFI;                             // 实例化WIFI_STA_AP类
Connect_Emqx *Emqx;
Ticker timer1;
Timer_Task Task1(1);
Timer_Task Task2(30);
httpclientData http_data;
void WifiConnectCallBack() // WIFI连接成功回调函数
{
  weather XinZhi(&Client);                                 // 心知天气
  XinZhi.weatherRequest(Weatherkey, "chengdu", http_data); // 发送天气请求
  Serial.printf("地址：成都; 天气：%s; 温度：%d摄氏度; \n",
                http_data.weather_txt.c_str(), http_data.temperature);
  httptime Gettime(&Client, http_data); // 获取时间
  Serial.printf("%d年%d月%d日 %d:%d:%d\n",
                http_data.year, http_data.month, http_data.day, http_data.hour, http_data.minute, http_data.second);
  Emqx = new Connect_Emqx;
  Emqx->clientReconnect();     // 连接mqtt服务器
  digitalWrite(LED_PIN, HIGH); // 关闭LED
}

void setup()
{
  p_WIFI_STA_AP = &WIFI; // 保存WIFI_STA_AP类的指针
  Serial.begin(115200);  // 设置串口波特率
  // LittleFS.format();     // 格式化SPIFFS
  Serial.println("SPIFFS format finish");
  if (LittleFS.begin())
  { // 启动SPIFFS
    Serial.println("SPIFFS Started.");
  }
  else
  {
    Serial.println("SPIFFS Failed to Start.");
  }
  File dataFile = LittleFS.open("/notes.txt", "r"); // 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  if (dataFile)                                     // 判断文件是否成功打开
  {
    // 读取文件内容并且通过串口监视器输出文件信息
    Serial.print(dataFile.readString());
    Serial.printf("大小：%d Byte\n", dataFile.size());
    dataFile.close(); // 完成文件写入后关闭文件
  }
  WIFI.WifiConnectCallBack = WifiConnectCallBack; // 设置WIFI连接回调函数
  WIFI.initSoftAP_STA();                          // 初始化WIFI SoftAP模式
  WIFI.initWebServer();                           // 初始化WIFI WebServer
  pinMode(LED_PIN, OUTPUT);                       // 设置GPIO模式为输出
  WIFI.connectNewWifi();                          // 连接WIFI
  timer1.attach(1, []()
                {Task1.Run();Task2.Run(); }); // 定时器1，每7秒执行一次
}

void loop()
{
  Emqx->getMQTTClient()->loop();           // mqtt客户端监听
  if (!Emqx->getMQTTClient()->connected()) // 看mqtt连接了没
  {
    Serial.println("mqtt disconnected!Try reconnect now...");
    Serial.println(Emqx->getMQTTClient()->state());
    Emqx->clientReconnect();
  }
  Task1.RunTask([]()
                { Serial.println(millis() / 1000); });
  Task2.RunTask([]() {});
}
