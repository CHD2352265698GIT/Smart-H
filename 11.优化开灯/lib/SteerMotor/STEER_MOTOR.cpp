#include <STEER_MOTOR.h>
#include <Ticker.h>
Ticker Timer0;
STEER_MOTOR steerMotor;               // 电机控制引脚
void STEER_MOTOR::setAngle(int angle) // 设置舵机转动角度
{
    steerMotor.attach(pin, 500, 2500); // 修正脉冲宽度
    if (angle < 0)
        angle = 0;
    else if (angle > 180)
        angle = 180;
    steerMotor.write(angle); // 0-180度对应0-255
    Serial.printf("  angle:%d\n", angle);
    Timer0.attach(1, []()
                  { Serial.println("释放控制引脚");
                    led.off();
                    steerMotor.detach(); Timer0.detach(); }); // 延时1s后释放控制引脚
}

void STEER_MOTOR::setLed(char on) // 设置LED状态
{
    if (on)
    {
        setAngle((int)OnValue);
        Serial.println("开");
    }
    else
    {
        setAngle((int)OffValue);
        Serial.println("关");
    }
}