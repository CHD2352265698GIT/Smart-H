#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Webserver.h>
extern IPAddress apIP;
extern DNSServer dnsServer;
extern char sta_ssid[32];
extern char sta_password[64];
void handleRootPost();
void handleRoot();
class WIFI_STA_AP
{
private:
    const byte DNS_PORT = 53;         // DNS端口号
    const char *AP_NAME = "配网WIFI"; // wifi名字
    // 暂时存储wifi账号密码

public:
    void initSoftAP(void)
    { // 初始化AP模式
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        if (WiFi.softAP(AP_NAME)) // 创建一个wifi
        {
            Serial.println("ESP8266 SoftAP is right"); // 打印AP是否成功
        }
    }
    void initDNS(void)
    { // 初始化DNS服务器
        if (dnsServer.start(DNS_PORT, "*", apIP))
        { // 判断将所有地址映射到esp8266的ip上是否成功
            Serial.println("start dnsserver success.");
        }
        else
            Serial.println("start dnsserver failed.");
    }
    void handleRootPost()
    { // Post回调函数,获取账号密码
        Serial.println("handleRootPost");
        if (esp8266_server.hasArg("ssid"))
        { // 判断是否有账号参数
            Serial.print("get ssid:");
            strcpy(sta_ssid, esp8266_server.arg("ssid").c_str()); // 将账号参数拷贝到sta_ssid中
            Serial.println(sta_ssid);
        }
        else
        { // 没有参数
            Serial.println("error, not found ssid");
            esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid"); // 返回错误页面
            return;
        }
        // 密码与账号同理
        if (esp8266_server.hasArg("password"))
        {
            Serial.print("get password:");
            strcpy(sta_password, esp8266_server.arg("password").c_str());
            Serial.println(sta_password);
        }
        else
        {
            Serial.println("error, not found password");
            esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
            return;
        }
        esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>保存成功"); // 返回保存成功页面
    }
    void initWebServer(void)
    { // 初始化WebServer
        // server.on("/",handleRoot);
        // 上面那行必须以下面这种格式去写否则无法强制门户
        esp8266_server.on("/", HTTP_GET, handleRoot);      // 设置主页回调函数
        esp8266_server.onNotFound(handleRoot);             // 设置无法响应的http请求的回调函数
        esp8266_server.on("/", HTTP_POST, handleRootPost); // 设置Post请求回调函数
        esp8266_server.begin();                            // 启动WebServer

        Serial.println("WebServer started!");
    }
};
