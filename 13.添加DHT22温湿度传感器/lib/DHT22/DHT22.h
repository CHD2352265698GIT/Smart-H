#pragma once
#include <Arduino.h>
#define DHT22_PIN 2 // DHT22数据引脚连接到ESP8266的GPIO0

class DHT22
{
public:
    DHT22(uint8_t pin = DHT22_PIN) // 构造函数
    {
        this->pin = pin;
    }
    float getTemperature(); // 获取温度
    float getHumidity();    // 获取湿度
    void printData();       // 打印数据
    char readData();        // 读取数据
    uint8_t pin;            // 数据引脚

private:
#define READPIN digitalRead(this->pin)
    float temperature;           // 温度
    float humidity;              // 湿度
    unsigned char Data[5] = {0}; // 存储读取的数据
    unsigned char ReadByte();    // 读取一个字节
};