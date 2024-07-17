#pragma once
#include <Arduino.h>
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库

#define HTMLSIZE 3161 - 1 // 网页内容大小

extern char index_html[HTMLSIZE]; // 网页内容
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
    } // 打印HTML内容
    void printHTML(void)
    {
        Serial.print(index_html);
    }
    void StartServer() // 启动HTTP服务器
    {
        esp8266_server.begin();                                                    // 启动HTTP服务器
        Serial.println("HTTP esp8266_server started");                             // 输出HTTP服务器启动信息
        esp8266_server.on("/", []()                                                // 发送HTTP响应头和网页内容
                          { esp8266_server.send(200, "text/html", index_html); }); // 处理“/”的访问请求
        esp8266_server.onNotFound([]()                                             // 当浏览器请求的网络资源无法在服务器找到时，处理404情况的函数
                                  { esp8266_server.send(404, "text/plain", "404: Not found"); });
    }
};
