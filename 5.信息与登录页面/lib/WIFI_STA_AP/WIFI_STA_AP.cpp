
#include <WIFI_STA_AP.h>

IPAddress apIP(192, 168, 4, 1); // esp8266-AP-IP地址
DNSServer dnsServer;            // 创建dnsServer实例
WIFI_STA_AP *p_WIFI_STA_AP;     // 创建WIFI_STA_AP实例指针

unsigned char Set_WIFI_SIGNED = 0; // 标志位，是否设置了wifi

void handleRoot()
{                                                                              // 访问主页回调函数
    char *html_Buffer = (char *)malloc(INDEX_HTML_SIZE);                       // malloc分配内存
    spi_flash_read(INDEX_HTML_SIZE, (uint32_t *)html_Buffer, INDEX_HTML_SIZE); // 读取网页内容到index_html中
    esp8266_server.send(200, "text/html", html_Buffer);                        // send返回html页面
    free(html_Buffer);                                                         // free内存
}

void handleRootPost()
{ // Post回调函数,获取账号密码
    Serial.println("handleRootPost");
    if (esp8266_server.hasArg("ssid"))
    { // 判断是否有账号参数
        Serial.print("get ssid:");
        strcpy(sta_ssid, esp8266_server.arg("ssid").c_str()); // 将账号参数拷贝到sta_ssid中
        sta_ssid_len = strlen(sta_ssid);                      // 计算账号长度
        Serial.println(sta_ssid);
        Serial.printf("len:%d\n", (int)sta_ssid_len);
    }
    else
    {                                                                                         // 没有参数
        Serial.println("error, not found ssid");                                              // 打印日志
        esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid"); // 返回错误页面
        return;
    }
    // 密码与账号同理
    if (esp8266_server.hasArg("password")) // 判断是否有密码参数
    {
        Serial.print("get password:");                                // 打印日志
        strcpy(sta_password, esp8266_server.arg("password").c_str()); // 将密码参数拷贝到sta_password中
        sta_password_len = strlen(sta_password);                      // 计算密码长度

        Serial.println(sta_password); // 打印日志
        Serial.printf("len:%d\n", (int)sta_password_len);
    }
    else
    {
        Serial.println("error, not found password");                                              // 打印日志
        esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password"); // 返回错误页面
        return;
    }
    esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>保存成功"); // 返回保存成功页面
    WiFi.begin(sta_ssid, sta_password);                                      // 连接STA模式的wifi
    Set_WIFI_SIGNED = 1;                                                     // 设置标志位，表示已经设置了密码
    p_WIFI_STA_AP->connectNewWifi();                                         // 连接新的wifi
}
