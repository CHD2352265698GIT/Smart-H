#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Webserver.h>
#include <LED.h>
#include <FLASH.h>
extern IPAddress apIP;                // AP模式的IP地址
extern DNSServer dnsServer;           // DNS服务器
extern char sta_ssid[32];             // 定义STA模式的wifi账号名
extern char sta_password[64];         // 定义STA模式的wifi账号密码
extern char sta_ssid_len;             // 账号长度
extern char sta_password_len;         // 密码长度
extern unsigned char Set_WIFI_SIGNED; // 定义设置了WIFI_SIGNED标志位
void handleRoot();
void handleRootPost();
void config();
class WIFI_STA_AP
{
private:
    const byte DNS_PORT = 53;        // DNS端口号
    const char *AP_NAME = "smart-H"; // wifi名字
public:
    void Write_WIFI_STA_AP_Config()
    {
        // 保存wifi信息到flash
        char temp_ss[sta_ssid_len + sta_password_len + 2] = {'\0'}; // 计算信息长度
        Serial.printf("%d,%d", (int)sta_ssid_len, (int)sta_password_len);
        spi_flash_erase_sector(FLASH_PAGE); // 先擦除之前保存的wifi信息
        temp_ss[0] = sta_ssid_len;
        temp_ss[1] = sta_password_len;
        strcat(temp_ss, sta_ssid);     // 连接账号
        strcat(temp_ss, sta_password); // 连接密码
        // 读写地址对齐：当从 Flash 读取或写入数据时，起始地址必须是4的倍数。比如可以从地址0x0000、0x0004、0x0008等开始读写，但不能从0x0001、0x0002等开始。
        spi_flash_write(FLASH_BASE_ADDR, (uint32_t *)temp_ss, sta_ssid_len + sta_password_len + 2); // 写入新的wifi信息
    }
    void Read_WIFI_STA_AP_Config()
    {
        // 读取wifi信息
        char temp[4];                                             // 临时存储wifi信息长度
        spi_flash_read(FLASH_BASE_ADDR, (uint32_t *)temp, 4);     // 读取之前保存的wifi信息
        sta_ssid_len = temp[0];                                   // 读取账号长度
        sta_password_len = temp[1];                               // 读取密码长度
        Serial.printf("sta_ssid_len:%d\n", sta_ssid_len);         // 打印账号长度
        Serial.printf("sta_password_len:%d\n", sta_password_len); // 打印密码长度
        int Information_len = sta_ssid_len + sta_password_len;
        char temp_ss[Information_len + 1] = {'\0'};                           // 计算信息长度
        spi_flash_read(WIFI_INFO_ADDR, (uint32_t *)temp_ss, Information_len); // 读取wifi信息
        memcpy(sta_ssid, temp_ss, sta_ssid_len);                              // 读取账号
        memcpy(sta_password, temp_ss + sta_ssid_len, sta_password_len);       // 读取密码
        Serial.print("\t");
        Serial.println(temp_ss);
        Serial.printf("sta_ssid: %s\n", sta_ssid);
        Serial.printf("sta_password: %s\n", sta_password);
    }
    void initSoftAP_STA(void)
    { // 初始化AP模式
        WiFi.mode(WIFI_AP_STA);
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

    void connectNewWifi(void)
    {
        WiFi.setAutoConnect(true); // 设置自动连接
        Serial.println("\n");
        Read_WIFI_STA_AP_Config();          // 读取flash保存的wifi信息
        WiFi.begin(sta_ssid, sta_password); // 连接上一次连接成功的wifi
        Serial.print("Connect to wifi");
        int count = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            led.off();
            delay(250);
            led.on();
            delay(250);
            count++;
            if (count > 20)
            {          // 如果5秒内没有连上，就开启Web配网 可适当调整这个时间
                break; // 跳出 防止无限初始化
            }
            Serial.print(".");
        }
        Serial.println("\n");
        if (WiFi.status() == WL_CONNECTED)
        { // 如果连接上 就输出IP信息 防止未连接上break后会误输出
            Serial.println("WIFI Connected!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP()); // 打印esp8266的IP地址
            // dnsServer.stop();               // 关闭DNS
            for (int i = 0; i < 500 * 3; i++) // 等待500ms
            {
                led.breathing(); // 呼吸灯
            }
            led.off(); // 熄灭LED
        }
    }
};
extern WIFI_STA_AP *p_WIFI_STA_AP; // 创建WIFI_STA_AP实例指针
