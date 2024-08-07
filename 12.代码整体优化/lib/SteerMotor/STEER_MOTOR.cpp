#include <STEER_MOTOR.h>
#include <Ticker.h>
Ticker Timer0;                        // 创建定时器
STEER_MOTOR steerMotor;               // 电机控制引脚
void STEER_MOTOR::setAngle(int angle) // 设置舵机转动角度
{
    steerMotor.attach(pin, 500, 2500); // 修正脉冲宽度
    if (angle < 0)                     // 判断是否超出范围
        angle = 0;
    else if (angle > 180)
        angle = 180;
    steerMotor.write(angle);              // 转动角度
    Serial.printf("  angle:%d\n", angle); // 打印角度
    Timer0.attach(1, []()
                  { Serial.println("释放控制引脚");
                    led.off();
                    steerMotor.detach(); Timer0.detach(); }); // 延时1s后释放控制引脚
}
void STEER_MOTOR::setLed(char on) // 设置LED状态
{
    if (!on) // 判断是否打开
    {
        setAngle((int)OnValue);     // 转到OnValue角度
        Serial.println("LIGHT-ON"); // 打印打开灯
    }
    else
    {
        setAngle((int)OffValue);     // 转到OffValue角度
        Serial.println("LIGHT-OFF"); // 打印关闭灯
    }
}
void STEER_MOTOR::setOnValue(char value) // 设置舵机开灯值
{
    OnValue = value;
}
void STEER_MOTOR::setOffValue(char value) // 设置舵机关灯值
{
    OffValue = value;
}