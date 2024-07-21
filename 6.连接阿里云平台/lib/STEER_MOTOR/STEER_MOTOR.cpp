#include <STEER_MOTOR.h>

STEER_MOTOR::STEER_MOTOR(int pin)
{
    pinMode(pin, OUTPUT);
    this->_pin = pin;
}

void STEER_MOTOR::turnToAngle(int SetAngle) // -100~100
{
    this->angle = SetAngle;
}
void STEER_MOTOR::service() // 控制电机转动
{
    analogWrite(this->_pin, angle);
}
void STEER_MOTOR::turnToAngle(int SetAngle, char mode) // 转动到指定角度
{
    switch (mode)
    {
    case 'N': // 正常模式
        analogWrite(this->_pin, SetAngle);
        break;
    }
}
