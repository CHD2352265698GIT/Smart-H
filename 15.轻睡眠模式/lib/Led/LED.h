#pragma once
#include <Arduino.h>
class LED
{
private:
#define LED_PIN 2       // 定义LED引脚
    int brightness = 0; // 定义亮度变量
    char state = 0;     // 定义状态变量
public:
    void toggle();             // 切换状态
    void blink(int number);    // 闪烁
    void breathing();          // 呼吸灯
    void on();                 // 打开
    void off();                // 关闭
    void setBrightness(int b); // 设置亮度
    int getBrightness();       // 获取亮度
};
extern LED led; // 实例化LED类