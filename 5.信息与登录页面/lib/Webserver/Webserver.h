#pragma once
#include <Arduino.h>
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库
#include <FLASH.h>

extern ESP8266WiFiMulti wifiMulti;
extern ESP8266WebServer esp8266_server;
class Webserver
{
private:
    const char *ssid_home = "ziroom2102";       // WiFi名称
    const char *password_home = "4001001111";   // WiFi密码
    const char *ssid_iphone = "chdwi";          // WiFi名称
    const char *password_iphone = "1234567890"; // WiFi密码
    int i = 0;                                  // 计数器
    char ssid[15] = {0};                        // WiFi名称
    char password[15] = {0};                    // 密码
public:
    void connect() // 连接WiFi
    {
        wifiMulti.addAP(ssid, password);               // 添加WiFi名称和密码
        wifiMulti.addAP(ssid_home, password_home);     // 添加WiFi名称和密码
        wifiMulti.addAP(ssid_iphone, password_iphone); // 添加WiFi名称和密码

        Serial.println("Connecting to WiFi");   // 输出连接WiFI信息
        while (wifiMulti.run() != WL_CONNECTED) // 等待WiFI连接成功
        {                                       // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
            delay(500);                         // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
            Serial.print("连接次数:\t");
            Serial.print(i++); // 将会自动选择连接信号最强的那一个WiFi信号。
        }
        // WiFi连接成功后将通过串口监视器输出连接成功信息
        Serial.println('\n'); // WiFi连接成功
    }
    // 打印连接WiFi名称
    void printSSID(void)
    {
        Serial.print("SSID:\t");
        Serial.println(WiFi.SSID());
    }
    // 打印IP地址
    void printIP(void)
    {
        Serial.print("IP address:\t");
        Serial.println(WiFi.localIP());
    }
};
