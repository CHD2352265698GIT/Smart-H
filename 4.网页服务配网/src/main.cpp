#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <LED.h>
#include <Webserver.h>
#include <WIFI_STA_AP.h>

LED led;             // 实例化LED类
Webserver webserver; // 实例化Webserver类
WIFI_STA_AP WIFI;    // 实例化WIFI_STA_AP类
void setup()
{
  Serial.begin(115200);                                      // 设置串口波特率
  pinMode(LED_PIN, OUTPUT);                                  // 设置GPIO模式为输出
  WIFI.initSoftAP();                                         // 初始化WIFI
  WIFI.initDNS();                                            // 初始化DNS
  WIFI.initWebServer();                                      // 初始化WebServer
  spi_flash_read(0x7c000, (uint32_t *)index_html, HTMLSIZE); // 读取网页内容到index_html中

  for (int i = 0; i < 500 * 3; i++) // 等待500ms
  {
    led.breathing(); // 呼吸灯
  }
  led.off(); // 熄灭LED
}

void loop()
{
  esp8266_server.handleClient();  // 处理http服务器访问
  dnsServer.processNextRequest(); // 处理DNS请求
}
