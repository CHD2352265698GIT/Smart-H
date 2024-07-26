#include <httpclient.h>
#include <ArduinoJson.h>

const char *location = "&location=%s&language=zh-Hans&unit=c HTTP/1.1\r\n";
weather::weather(WiFiClient *espClient)
{
    this->espClient = espClient;
    espClient->setTimeout(5000); // Set the timeout to 5 seconds
}
void weather::weatherRequest(const char *key, const char *Request_location, httpclientData &result)
{
    if (espClient->connect("api.seniverse.com", 80)) // 连接服务器
    {
        espClient->print("GET /v3/weather/now.json?key="); // 请求地址
        espClient->print(key);                             // 请求地址
        char *str = (char *)malloc(100);
        if (str == NULL)
        {
            Serial.println("malloc failed");
            return;
        }
        sprintf(str, location, Request_location);
        espClient->print(str);
        espClient->print("Host: api.seniverse.com\r\n\r\n");
        espClient->print("Accept-Language:zh-cn\r\n");
        espClient->print("Connection:close\r\n\r\n");
        free(str);
        String response = espClient->readStringUntil('\n'); // 读取 第一行响应
        Serial.println(response);
        if (espClient->find("\r\n\r\n"))
        {
            String Json_form_server = espClient->readStringUntil('\n'); // 读取Json数据
            Serial.println("请求成功");
            Serial.println(Json_form_server);
            AnalysisJson(Json_form_server.c_str(), result); // 解析Json数据
        }
    }
}

void weather::AnalysisJson(const char *json, httpclientData &result)
{
    JsonDocument doc;
    deserializeJson(doc, json);
    result.temperature = doc["results"][0]["now"]["temperature"].as<String>().toInt();
    result.weather_txt = doc["results"][0]["now"]["text"].as<String>();
    result.weather_code = doc["results"][0]["now"]["code"].as<String>().toInt();
    Serial.println("解析成功:");
}

httptime::httptime(WiFiClient *espClient, httpclientData &result)
{
    if (espClient->connect("quan.suning.com", 80)) // 连接服务器
    {
        Serial.println("连接成功");
        espClient->print("GET /getSysTime.do HTTP/1.1\r\n"); // 请求地址
        espClient->print("Host: quan.suning.com\r\n\r\n");

        String response = espClient->readStringUntil('\n'); // 读取 第一行响应
        Serial.println(response);
        if (espClient->find("\r\n\r\n"))
        {
            String Json_form_server = espClient->readStringUntil('\n'); // 读取Json数据
            Serial.println("请求成功");
            Serial.println(Json_form_server);
            // 解析Json数据
            JsonDocument doc;
            deserializeJson(doc, Json_form_server);
            String time = doc["sysTime2"].as<String>();
            sscanf(time.c_str(), "%d-%d-%d %d:%d:%d", &result.year, &result.month, &result.day, &result.hour, &result.minute, &result.second);
            Serial.println("解析成功:");
            Serial.printf("%d年%d月%d日 %d:%d:%d\n", result.year, result.month, result.day, result.hour, result.minute, result.second);
        }
    }
}