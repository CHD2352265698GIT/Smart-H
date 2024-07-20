
#include <WIFI_STA_AP.h>

IPAddress apIP(192, 168, 4, 1); // esp8266-AP-IP地址
DNSServer dnsServer;            // 创建dnsServer实例
WIFI_STA_AP *p_WIFI_STA_AP;     // 创建WIFI_STA_AP实例指针

unsigned char Set_WIFI_SIGNED = 0;             // 标志位，是否设置了wifi
const static String LOGIN_USERNAME = "admin";  // 登录用户名
const static String LOGIN_PASSWORD = "123456"; // 登录密码
void handleRoot()
{ // 访问主页回调函数
    char *html_Buffer = (char *)malloc(LOGIN_HTML_SIZE + 1);
    spi_flash_read(LOGIN_HTML_ADDR, (uint32_t *)html_Buffer, LOGIN_HTML_SIZE); // 读取网页内容到html_Buffer中
    html_Buffer[LOGIN_HTML_SIZE] = '\0';
    esp8266_server.send(200, "text/html", html_Buffer); // send返回html页面
    free(html_Buffer);                                  // 释放html_Buffer
}

void handleRootPost()
{
    // Post回调函数,检查是否有账号密码参数
    Serial.println("handleRootPost");
    if (esp8266_server.hasArg("username"))
    {
        Serial.print("get username:");
        String temp_username = esp8266_server.arg("username"); // 获取账号参数
        Serial.println(temp_username);
        if (temp_username == LOGIN_USERNAME) // 判断账号是否正确
        {
            if (esp8266_server.hasArg("password"))
            {
                Serial.print("get password:");
                String temp_password = esp8266_server.arg("password"); // 获取密码参数
                Serial.println(temp_password);
                if (temp_password == LOGIN_PASSWORD) // 判断密码是否正确
                {
                    esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>Login Success"); // 登录成功，返回登录成功页面
                    return;
                }
                else
                {
                    esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>密码错误"); // 登录失败，返回登录失败页面
                    return;
                }
            }
            else
            {
                esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>缺少参数"); // 登录失败，返回登录失败页面
                return;
            }
        }
        else
        {
            esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>无该账号"); // 登录失败，返回登录失败页面
            return;
        }
    }
    else
    {
        esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>缺少参数"); // 登录失败，返回登录失败页面
        return;
    }
}

void handleConfigPost()
{
    // Post回调函数,检查是否有账号密码参数
    Serial.println("handleConfigPost");
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

    Set_WIFI_SIGNED = 1;             // 设置标志位，表示已经设置了密码
    p_WIFI_STA_AP->connectNewWifi(); // 连接新的wifi
}
void config()
{
    char html_Buffer[CONFIG_HTML_SIZE + 1];                                      // 建立buffer，用于存放html页面
    spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
    html_Buffer[CONFIG_HTML_SIZE] = '\0';                                        // 添加字符串结束符
    esp8266_server.send(200, "text/html", html_Buffer);                          // send返回html页面
}