#include <Arduino.h>
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库

/* 引脚输出PWM与网页服务 */
ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'

ESP8266WebServer esp8266_server(80); // 建立ESP8266WebServer对象，对象名称为esp8266_server
                                     // 括号中的数字是网路服务器响应http请求的端口号
                                     // 网络服务器标准http端口号为80，因此这里使用80为端口号
#define HTMLSIZE 1537                // 网页内容大小

class LED
{
private:
#define LED_PIN 2     // 定义LED引脚
  int brightness = 0; // 定义亮度变量
  char state = 0;     // 定义状态变量
public:
  // 闪烁
  void blink(int number, int Brightness1, int Brightness2)
  {
    for (int i = 0; i < number; i++)
    {
      setBrightness(Brightness1);
      delay(200);
      setBrightness(Brightness2);
      delay(200);
    }
    setBrightness(255);
  }
  void on()
  {
    setBrightness(0);
  }
  void off()
  {
    setBrightness(255);
  }
  void setBrightness(int b)
  {
    brightness = b;
    analogWrite(LED_PIN, brightness);
  }
  int getBrightness()
  {
    return brightness;
  }
  // 呼吸灯
  void breathing()
  {
    if (state == 0)
    {
      brightness = brightness + 1;
      if (brightness > 255)
      {
        brightness = 255;
        state = 1;
      }
      analogWrite(LED_PIN, brightness);
    }
    else
    {
      brightness = brightness - 1;
      if (brightness < 0)
      {
        brightness = 0;
        state = 0;
      }
      analogWrite(LED_PIN, brightness);
    }
  }
};
char index_html[HTMLSIZE] = {0}; // 定义网页内容数组,大小为10KB
class Webserver
{
private:
  const char *ssid = "ziroom2102";     // WiFi名称
  const char *password = "4001001111"; // WiFi密码
  int i = 0;                           // 计数器
public:
  void connect() // 连接WiFi
  {
    wifiMulti.addAP(ssid, password);        // 添加WiFi名称和密码
    Serial.println("Connecting to WiFi");   // 输出连接WiFI信息
    while (wifiMulti.run() != WL_CONNECTED) // 等待WiFI连接成功
    {                                       // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
      delay(1000);                          // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
      Serial.print('\t');
      Serial.print(i++); // 将会自动选择连接信号最强的那一个WiFi信号。
    }
    // WiFi连接成功后将通过串口监视器输出连接成功信息
    Serial.println('\n');           // WiFi连接成功后
    Serial.print("Connected to ");  // NodeMCU将通过串口监视器输出。
    Serial.println(WiFi.SSID());    // 连接的WiFI名称
    Serial.print("IP address:\t");  // 以及
    Serial.println(WiFi.localIP()); // NodeMCU的IP地址
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

LED led;             // 实例化LED类
Webserver webserver; // 实例化Webserver类

void setup()
{
  Serial.begin(115200);     // 设置串口波特率
  pinMode(LED_PIN, OUTPUT); // 设置GPIO模式为输出

  spi_flash_read(0x6c000, (uint32_t *)index_html, HTMLSIZE); // 读取网页内容到index_html中

  webserver.connect();              // 连接WiFi
  webserver.StartServer();          // 启动HTTP服务器后，浏览器访问http://192.168.4.1/ 即可看到网页内容
  for (int i = 0; i < 500 * 2; i++) // 等待500ms
  {
    led.breathing(); // 呼吸灯
  }
  led.off(); // 熄灭LED
}

void loop()
{
  esp8266_server.handleClient(); // 处理http服务器访问
}
