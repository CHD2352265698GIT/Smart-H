
#include <WIFI_STA_AP.h>

IPAddress apIP(192, 168, 0, 1);                // esp8266-AP-IP地址
DNSServer dnsServer;                           // 创建dnsServer实例
WIFI_STA_AP *p_WIFI_STA_AP;                    // 创建WIFI_STA_AP实例指针
ESP8266WebServer esp8266_server(80);           // 建立ESP8266WebServer对象，对象名称为esp8266_server
                                               // 括号中的数字是网路服务器响应http请求的端口号
                                               // 网络服务器标准http端口号为80，因此这里使用80为端口号
const static String LOGIN_USERNAME = "admin";  // 登录用户名
const static String LOGIN_PASSWORD = "123456"; // 登录密码
static char Set_wifi_flag = 0;                 // 标志位，是否设置了wifi
char sta_ssid[32] = {0};
char sta_password[64] = {0};
char sta_ssid_len = 0;     // 账号长度
char sta_password_len = 0; // 密码长度
void handleRoot()
{ // 访问主页回调函数
    char *html_Buffer = (char *)malloc(LOGIN_HTML_SIZE + 1);
    spi_flash_read(LOGIN_HTML_ADDR, (uint32_t *)html_Buffer, LOGIN_HTML_SIZE); // 读取网页内容到html_Buffer中
    html_Buffer[LOGIN_HTML_SIZE] = '\0';                                       // 添加字符串结束符
    esp8266_server.send(200, "text/html", html_Buffer);                        // send返回html页面
    free(html_Buffer);                                                         // 释放html_Buffer
    Set_wifi_flag = 0;                                                         // 重置标志位
}

void handleConfigPost(char &flag)
{
    // Post回调函数,检查是否有账号密码参数
    Serial.println("handleConfigPost");
    if (esp8266_server.hasArg("angle"))
    {
        int angle = esp8266_server.arg("angle").toInt(); // 获取角度值
        steerMotor.write(angle);
        delay(1000);
        // 停止脉冲
        steerMotor.detach();
        char *html_Buffer = (char *)malloc(CONFIG_HTML_SIZE + 1);
        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
        html_Buffer[CONFIG_HTML_SIZE] = '\0';                                        // 添加字符串结束符
        esp8266_server.send(200, "text/html", html_Buffer);                          // 登录成功，返回登录成功页面
        free(html_Buffer);
    }
    else
    {

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
        flag = 0;                                  // 设置标志位，表示已经设置了wifi
        p_WIFI_STA_AP->Write_WIFI_STA_AP_Config(); // 保存wifi信息到flash
        p_WIFI_STA_AP->connectNewWifi();           // 连接新的wifi
    }
}
void handleRootPost()
{
    // Post回调函数,检查是否有账号密码参数
    Serial.println("handleRootPost");
    if (Set_wifi_flag == 1)
    {
        handleConfigPost(Set_wifi_flag); // 跳转到检查
    }
    else
    {
        if (esp8266_server.hasArg("username"))
        {
            Serial.print("get username:");
            String temp_username = esp8266_server.arg("username"); // 获取账号参数
            Serial.println(temp_username);
            if (temp_username == LOGIN_USERNAME) // 判断账号是否正确
            {
                if (esp8266_server.hasArg("userpassword"))
                {
                    Serial.print("get userpassword:");
                    String temp_password = esp8266_server.arg("userpassword"); // 获取密码参数
                    Serial.println(temp_password);
                    if (temp_password == LOGIN_PASSWORD) // 判断密码是否正确
                    {
                        char *html_Buffer = (char *)malloc(CONFIG_HTML_SIZE + 1);
                        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
                        html_Buffer[CONFIG_HTML_SIZE] = '\0';                                        // 添加字符串结束符
                        esp8266_server.send(200, "text/html", html_Buffer);                          // 登录成功，返回登录成功页面
                        free(html_Buffer);
                        Set_wifi_flag = 1;               // 设置标志位，表示已经设置了密码
                        Serial.println("Login Success"); // 登录成功，返回登录成功页面
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
                    esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>缺少参数 userpassword"); // 登录失败，返回登录失败页面
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
            esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>缺少参数 username"); // 登录失败，返回登录失败页面
            return;
        }
    }
}
