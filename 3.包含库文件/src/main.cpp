#include <Arduino.h>
#include <ESP8266WiFiMulti.h> //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h> //  ESP8266WebServer库
#include <LED.h>
#include <Webserver.h>

LED led;             // 实例化LED类
Webserver webserver; // 实例化Webserver类

void setup()
{
  Serial.begin(115200);     // 设置串口波特率
  pinMode(LED_PIN, OUTPUT); // 设置GPIO模式为输出

  spi_flash_read(0x6c000, (uint32_t *)index_html, HTMLSIZE); // 读取网页内容到index_html中

  webserver.connect();              // 连接WiFi
  webserver.StartServer();          // 启动HTTP服务器后，浏览器访问http://192.168.4.1/ 即可看到网页内容
  for (int i = 0; i < 500 * 3; i++) // 等待500ms
  {
    led.breathing(); // 呼吸灯
  }
  led.off(); // 熄灭LED
}

void loop()
{
  esp8266_server.handleClient(); // 处理http服务器访问
}
