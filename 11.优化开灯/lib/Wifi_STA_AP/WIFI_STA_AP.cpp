
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
char sta_ssid_len = 0;                  // 账号长度
char sta_password_len = 0;              // 密码长度
char html_Buffer[CONFIG_HTML_SIZE + 1]; // 定义一个缓冲区用于存放网页内容
void handleRoot()
{
    memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                           // 清空缓冲区
    spi_flash_read(LOGIN_HTML_ADDR, (uint32_t *)html_Buffer, LOGIN_HTML_SIZE); // 读取网页内容到html_Buffer中
    esp8266_server.send(200, "text/html", html_Buffer);
    Set_wifi_flag = 0; // 重置标志位
}

void handleConfigPost(char &flag)
{
    // Post回调函数,检查是否有账号密码参数
    Serial.println("handleConfigPost");
    if (esp8266_server.hasArg("on_angle"))
    {
        // 处理角度值
        int angle = esp8266_server.arg("on_angle").toInt(); // 获取角度值
        p_WIFI_STA_AP->on_value = angle;                    // 调用on_value函数
        angle = esp8266_server.arg("off_angle").toInt();    // 获取角度值
        p_WIFI_STA_AP->off_value = angle;                   // 调用off_value函数
        // 存储角度值到flash
        spi_flash_erase_sector(WIFI_MOTOR_SECTOR_ADDR); // 先擦除flash扇区
        char temp_buffer[4];
        temp_buffer[0] = p_WIFI_STA_AP->on_value;
        temp_buffer[1] = p_WIFI_STA_AP->off_value;
        spi_flash_write(WIFI_MOTOR_ANGLE_ADDR, (uint32_t *)temp_buffer, 4); // 写入on_value到flash
        spi_flash_read(WIFI_MOTOR_ANGLE_ADDR, (uint32_t *)temp_buffer, 2);  // 读取舵机角度
        Serial.printf("On:%d,Off:%d\n", (int)temp_buffer[0], (int)temp_buffer[1]);
        memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                             // 清空缓冲区
        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
        esp8266_server.send(200, "text/html", html_Buffer);                          // send返回html页面
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
            Serial.println(sta_password);                                 // 打印日志
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
                        memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                             // 清空缓冲区
                        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
                        esp8266_server.send(200, "text/html", html_Buffer);                          // 登录成功，返回登录成功页面
                        Set_wifi_flag = 1;                                                           // 设置标志位，表示已经设置了密码
                        Serial.println("Login Success");                                             // 登录成功，返回登录成功页面
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

void WIFI_STA_AP::connectNewWifi()
{
    WiFi.setAutoConnect(true); // 设置自动连接
    Serial.println("\n");
    Read_WIFI_STA_AP_Config();          // 读取flash保存的wifi信息
    WiFi.begin(sta_ssid, sta_password); // 连接上一次连接成功的wifi
    Serial.print("Connect to wifi");

    Ticker timer1; // 实例化定时器类
    timer1.attach_ms(250, []()
                     { led.toggle(); Serial.print("."); }); // 每隔0.5s切换一次LED状态
    while (WiFi.status() != WL_CONNECTED)
    {
        esp8266_server.handleClient(); // 处理客户端请求
    }
    timer1.detach(); // 关闭定时器
    Serial.println("\n");
    if (WiFi.status() == WL_CONNECTED)
    { // 如果连接上 就输出IP信息 防止未连接上break后会误输出
        Serial.println("WIFI Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); // 打印esp8266的IP地址
        WiFi.mode(WIFI_STA);            // 切换回STA模式
        led.blink(6);                   // 闪烁6次LED
        WifiConnectCallBack();          // 连接成功后回调函数
    }
}