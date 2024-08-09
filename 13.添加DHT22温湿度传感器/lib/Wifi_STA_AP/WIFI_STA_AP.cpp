
#include <WIFI_STA_AP.h>

static IPAddress apIP(192, 168, 0, 1);                // esp8266-AP-IP地址
static DNSServer dnsServer;                           // 创建dnsServer实例
static ESP8266WebServer esp8266_server(80);           // 建立ESP8266WebServer对象，对象名称为esp8266_server
                                                      // 括号中的数字是网路服务器响应http请求的端口号
                                                      // 网络服务器标准http端口号为80，因此这里使用80为端口号
const static String LOGIN_USERNAME = "admin";         // 登录用户名
const static String LOGIN_PASSWORD = "123456";        // 登录密码
static char Set_wifi_flag = 0;                        // 标志位，是否设置了wifi
static char sta_ssid[32] = {0};                       // 账号
static char sta_password[64] = {0};                   // 密码
static char sta_ssid_len = 0;                         // 账号长度
static char sta_password_len = 0;                     // 密码长度
static char html_Buffer[CONFIG_HTML_SIZE + 1];        // 定义一个缓冲区用于存放网页内容
static String error_message = "<meta charset='UTF-8'><h1 style=\" \
position : absolute;top : 40 \% ;left : 50 \% ;transform : \
translate(-50 \%, -50 \%);text - align : center;\">"; // 错误信息
WIFI_STA_AP *p_WIFI_STA_AP;                           // 创建WIFI_STA_AP实例指针
static void handleRoot()                              // 根目录回调函数
{
    memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                           // 清空缓冲区
    spi_flash_read(LOGIN_HTML_ADDR, (uint32_t *)html_Buffer, LOGIN_HTML_SIZE); // 读取网页内容到html_Buffer中
    esp8266_server.send(200, "text/html", html_Buffer);                        // send返回html页面
    Set_wifi_flag = 0;                                                         // 重置标志位
}
static void handleConfigPost(char &flag) // Post回调函数,检查各项参数
{
    Serial.println("handleConfigPost");
    if (esp8266_server.hasArg("on_angle")) // 判断是否有角度参数
    {
        char temp_buffer[4];                                                         // 定义一个临时缓冲区用于存放舵机角度值
        memset(temp_buffer, '\0', 4);                                                // 清空缓冲区
        int angle = esp8266_server.arg("on_angle").toInt();                          // 获取角度值
        temp_buffer[0] = angle;                                                      // 调用on_value函数
        angle = esp8266_server.arg("off_angle").toInt();                             // 获取角度值
        temp_buffer[1] = angle;                                                      // 调用off_value函数
        spi_flash_erase_sector(WIFI_MOTOR_SECTOR_ADDR);                              // 先擦除flash扇区
        spi_flash_write(WIFI_MOTOR_ANGLE_ADDR, (uint32_t *)temp_buffer, 4);          // 写入on_value到flash
        spi_flash_read(WIFI_MOTOR_ANGLE_ADDR, (uint32_t *)temp_buffer, 2);           // 读取舵机角度
        Serial.printf("On:%d,Off:%d\n", (int)temp_buffer[0], (int)temp_buffer[1]);   // 打印舵机角度
        memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                             // 清空缓冲区
        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
        esp8266_server.send(200, "text/html", html_Buffer);                          // send返回html页面
    }
    else
    {
        if (esp8266_server.hasArg("ssid")) // 判断是否有账号参数
        {
            Serial.print("get ssid:");                            // 打印日志
            strcpy(sta_ssid, esp8266_server.arg("ssid").c_str()); // 将账号参数拷贝到sta_ssid中
            sta_ssid_len = strlen(sta_ssid);                      // 计算账号长度
            Serial.println(sta_ssid);                             // 打印日志
            Serial.printf("len:%d\n", (int)sta_ssid_len);
        }
        else
        {                                                                                         // 没有参数
            Serial.println("error, not found ssid");                                              // 打印日志
            esp8266_server.send(200, "text/html", "<meta charset='UTF-8'>error, not found ssid"); // 返回错误页面
            return;
        }
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
static void handleRootPost() // Post回调函数,检查是否有账号密码参数
{
    Serial.println("handleRootPost"); // 打印日志
    if (Set_wifi_flag == 1)           // 如果输入密码正确，则跳转到设置页面
    {
        handleConfigPost(Set_wifi_flag); // 调用handleConfigPost函数
    }
    else
    {
        if (esp8266_server.hasArg("username")) // 判断是否有用户名参数
        {
            Serial.print("get username:");                         // 打印日志
            String temp_username = esp8266_server.arg("username"); // 获取账号参数
            Serial.println(temp_username);                         // 打印用户名
            if (temp_username == LOGIN_USERNAME)                   // 判断用户名是否正确
            {
                if (esp8266_server.hasArg("userpassword")) // 判断是否有密码参数
                {
                    Serial.print("get userpassword:");                         // 打印日志
                    String temp_password = esp8266_server.arg("userpassword"); // 获取密码参数
                    Serial.println(temp_password);                             // 打印密码
                    if (temp_password == LOGIN_PASSWORD)                       // 判断密码是否正确
                    {
                        memset(html_Buffer, '\0', CONFIG_HTML_SIZE + 1);                             // 清空缓冲区
                        spi_flash_read(CONFIG_HTML_ADDR, (uint32_t *)html_Buffer, CONFIG_HTML_SIZE); // 读取网页内容到html_Buffer中
                        esp8266_server.send(200, "text/html", html_Buffer);                          // 登录成功，发送设置页面
                        Set_wifi_flag = 1;                                                           // 设置标志位，表示已经登录成功
                        Serial.println("Login Success");                                             // 打印日志
                        return;
                    }
                    else
                    {
                        esp8266_server.send(200, "text/html", error_message + "密码错误</h1>"); // 登录失败，返回登录失败页面
                        return;
                    }
                }
                else
                {
                    esp8266_server.send(200, "text/html", error_message + "缺少密码参数</h1>"); // 登录失败，返回登录失败页面
                    return;
                }
            }
            else
            {
                esp8266_server.send(200, "text/html", error_message + "无该账号</h1>"); // 登录失败，返回登录失败页面
                return;
            }
        }
        else
        {
            esp8266_server.send(200, "text/html", error_message + "缺少参数</h1>"); // 登录失败，返回登录失败页面
            return;
        }
    }
}
void WIFI_STA_AP::connectNewWifi() // 读取flash保存的wifi信息后连接
{
    WiFi.setAutoConnect(true); // 设置自动连接
    Serial.println("\n");
    Read_WIFI_STA_AP_Config();          // 读取flash保存的wifi信息
    WiFi.begin(sta_ssid, sta_password); // 连接上一次连接成功的wifi
    Serial.print("Connect to wifi");    // 打印日志
    Ticker timer1;                      // 实例化定时器类
    timer1.attach_ms(250, []()
                     { led.toggle(); Serial.print("."); });                // 定时每隔0.5s切换一次LED状态
    while (WiFi.status() != WL_CONNECTED) // 如果未连接上wifi
    {
        esp8266_server.handleClient(); // 死循环处理客户端请求
    }
    timer1.detach(); // 关闭定时器
    Serial.println("\n");
    if (WiFi.status() == WL_CONNECTED) // 如果连接上 就输出IP信息 防止未连接上break后会误输出
    {
        Serial.println("WIFI Connected!"); // 打印日志
        Serial.print("IP address: ");      // 打印日志
        Serial.println(WiFi.localIP());    // 打印esp8266的IP地址
        WiFi.mode(WIFI_STA);               // 切换回STA模式
        led.blink(6);                      // 闪烁6次LED
        WifiConnectCallBack();             // 连接成功后回调函数
    }
}
void WIFI_STA_AP::Write_WIFI_STA_AP_Config() // 写入wifi信息到flash
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
void WIFI_STA_AP::Read_WIFI_STA_AP_Config() // 从flash读取wifi信息
{
    char temp[4];                                         // 临时存储wifi信息长度
    spi_flash_read(FLASH_BASE_ADDR, (uint32_t *)temp, 4); // 读取之前保存的wifi信息
    sta_ssid_len = temp[0];                               // 读取账号长度
    sta_password_len = temp[1];                           // 读取密码长度
    int Information_len = sta_ssid_len + sta_password_len;
    char temp_ss[Information_len + 1] = {'\0'};                           // 计算信息长度
    spi_flash_read(WIFI_INFO_ADDR, (uint32_t *)temp_ss, Information_len); // 读取wifi信息
    memcpy(sta_ssid, temp_ss, sta_ssid_len);                              // 读取账号
    memcpy(sta_password, temp_ss + sta_ssid_len, sta_password_len);       // 读取密码
    temp_ss[Information_len] = '\0';                                      // 添加字符串结束符
    Serial.printf("sta_ssid: %s\n", sta_ssid);                            // 打印账号
    Serial.printf("sta_password: %s\n", sta_password);                    // 打印密码
}
void WIFI_STA_AP::initSoftAP_STA(void) // 初始化AP模式
{
    WiFi.mode(WIFI_AP_STA);                                     // 切换到AP模式
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // 设置ap的IP和网关
    if (WiFi.softAP(AP_NAME))                                   // 创建一个wifi
    {
        Serial.println("\n\nESP8266 SoftAP is right"); // 打印AP是否成功
    }
}
void WIFI_STA_AP::initDNS(void) // 初始化DNS服务器
{
    if (dnsServer.start(DNS_PORT, "*", apIP)) // 判断将所有地址映射到esp8266的ip上是否成功
    {
        Serial.println("start dnsserver success."); // 打印DNS是否启动
    }
    else
        Serial.println("start dnsserver failed."); // 打印DNS是否启动
}
void WIFI_STA_AP::initWebServer(void) // 初始化WebServer
{
    // server.on("/",handleRoot);
    // 上面那行必须以下面这种格式去写否则无法强制门户
    esp8266_server.on("/", HTTP_GET, handleRoot);      // 设置主页回调函数
    esp8266_server.onNotFound(handleRoot);             // 设置无法响应的http请求的回调函数
    esp8266_server.on("/", HTTP_POST, handleRootPost); // 设置Post请求回调函数
    esp8266_server.begin();                            // 启动WebServer
    Serial.println("WebServer started!");              // 打印WebServer是否启动
}