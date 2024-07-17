#include <Webserver.h>

char index_html[HTMLSIZE] = {0}; // 定义网页内容数组,大小为10KB

/* 引脚输出PWM与网页服务 */
ESP8266WiFiMulti wifiMulti;          // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
ESP8266WebServer esp8266_server(80); // 建立ESP8266WebServer对象，对象名称为esp8266_server
                                     // 括号中的数字是网路服务器响应http请求的端口号
                                     // 网络服务器标准http端口号为80，因此这里使用80为端口号