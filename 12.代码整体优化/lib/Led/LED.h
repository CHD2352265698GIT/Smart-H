#pragma once

#include <Arduino.h>
class LED
{
private:
#define LED_PIN 2       // 定义LED引脚
    int brightness = 0; // 定义亮度变量
    char state = 0;     // 定义状态变量
public:
    void toggle()
    {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    // 闪烁
    void blink(int number)
    {
        for (int i = 0; i < number; i++) // 等待500ms
        {
            on(); // 熄灭LED
            delay(50);
            off(); // 熄灭LED
            delay(50);
        }
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
extern LED led; // 实例化LED类