#include <httpclient.h>
#include <ArduinoJson.h>

const char *location = "&location=%s&language=zh-Hans&unit=c HTTP/1.1\r\n";
weather::weather(WiFiClient *espClient)
{
    this->espClient = espClient;
    espClient->setTimeout(5000); // Set the timeout to 5 seconds
}
void weather::weatherRequest(const char *key, const char *Request_location, weather_Data &result)
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

void weather::AnalysisJson(const char *json, weather_Data &result)
{
    JsonDocument doc;
    deserializeJson(doc, json);
    result.temperature = doc["results"][0]["now"]["temperature"].as<String>().toInt();
    result.weather_txt = doc["results"][0]["now"]["text"].as<String>();
    result.weather_code = doc["results"][0]["now"]["code"].as<String>().toInt();
    Serial.println("解析成功:");
}