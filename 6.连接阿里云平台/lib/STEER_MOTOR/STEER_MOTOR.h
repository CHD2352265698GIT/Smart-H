#pragma once

#include <Arduino.h>

#define STEER_MOTOR_MAX_ANGLE 180     // 舵机最大转动角度
#define STEER_MOTOR_MIN_ANGLE 0       // 舵机最小转动角度
#define STEER_MOTOR_DEFAULT_SPEED 100 // 舵机默认转动速度
#define STEER_MOTOR_PIN 2             // 舵机控制引脚
class STEER_MOTOR
{
public:
    STEER_MOTOR(int pin);                      // 构造函数，传入舵机控制引脚
    void turnToAngle(int SetAngle);            // 转动到指定角度
    void turnToAngle(int SetAngle, char mode); // 转动到指定角度
    void setSpeed(int speed);                  // 设置转动速度
    void stop();                               // 停止输出
    void service();                            // 控制电机转动
private:
    int _pin;  // 舵机控制引脚
    int angle; // 舵机当前角度
};
