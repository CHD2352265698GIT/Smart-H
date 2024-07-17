
#include <WIFI_STA_AP.h>

IPAddress apIP(192, 168, 4, 1); // esp8266-AP-IP地址
DNSServer dnsServer;            // 创建dnsServer实例

void handleRoot()
{ // 访问主页回调函数
    esp8266_server.send(200, "text/html", index_html);
}
