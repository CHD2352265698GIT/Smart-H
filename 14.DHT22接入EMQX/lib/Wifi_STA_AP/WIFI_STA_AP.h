#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LED.h>
#include <FLASH.h>
#include <STEER_MOTOR.h>
#include <Ticker.h>
class WIFI_STA_AP
{
private:
    const byte DNS_PORT = 53;        // DNS端口号
    const char *AP_NAME = "smart-H"; // wifi名字
public:
    void (*WifiConnectCallBack)(void); // 回调函数
    void Write_WIFI_STA_AP_Config();   // 写入wifi信息到flash
    void Read_WIFI_STA_AP_Config();    // 从flash读取wifi信息
    void initSoftAP_STA(void);         // 初始化AP模式
    void initDNS(void);                // 初始化DNS服务器
    void initWebServer(void);          // 初始化WebServer
    void connectNewWifi();             // 读取flash保存的wifi信息后连接
};
extern WIFI_STA_AP *p_WIFI_STA_AP; // 创建WIFI_STA_AP实例指针
