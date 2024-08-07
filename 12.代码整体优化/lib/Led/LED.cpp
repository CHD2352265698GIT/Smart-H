#include <LED.h>
LED led;           // 实例化LED类
void LED::toggle() // 切换状态
{
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // 切换LED状态
}
void LED::blink(int number) // 闪烁
{
    for (int i = 0; i < number; i++) // 闪烁次数
    {
        on();      // 熄灭LED
        delay(50); // 等待500ms
        off();     // 熄灭LED
        delay(50); // 等待500ms
    }
}
void LED::on() // 点亮
{
    setBrightness(0);
}
void LED::off() // 熄灭
{
    setBrightness(255);
}
void LED::setBrightness(int b) // 设置亮度
{
    brightness = b;
    analogWrite(LED_PIN, brightness);
}
int LED::getBrightness() // 获取亮度
{
    return brightness;
}
void LED::breathing() // 呼吸灯
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
