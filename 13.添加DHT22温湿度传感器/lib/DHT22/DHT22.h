#pragma once
#include <Arduino.h>
class DHT22
{
public:
    DHT22(uint8_t pin = DHT22_PIN) // 构造函数
    {
        this->pin = pin;
    }
    float getTemperature();  // 获取温度
    float getHumidity();     // 获取湿度
    const char *ErrorData(); // 错误数据
    char readData();         // 读取数据
    uint8_t pin;             // 数据引脚
private:
#define DHT22_PIN 2                    // DHT22数据默认引脚
#define READPIN digitalRead(this->pin) // 读取引脚电平
    char symbol = 0;                   // 符号位
    char errorNumber = 0;              // 错误码
    float temperature;                 // 温度
    float humidity;                    // 湿度
    unsigned char Data[5] = {0};       // 存储读取的数据
    unsigned char ReadByte();          // 读取一个字节
};