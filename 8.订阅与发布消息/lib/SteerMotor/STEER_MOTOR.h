#pragma once

#include <Arduino.h>
#include <Servo.h>

#define STEER_MOTOR_MAX_ANGLE 180     // 舵机最大转动角度
#define STEER_MOTOR_MIN_ANGLE 0       // 舵机最小转动角度
#define STEER_MOTOR_DEFAULT_SPEED 100 // 舵机默认转动速度
#define STEER_MOTOR_PIN 2             // 舵机控制引脚

class STEER_MOTOR : public Servo
{
public:
    STEER_MOTOR(int pin = STEER_MOTOR_PIN) // 构造函数
    {
        attach(pin, 500, 2500); // 修正脉冲宽度
    }
};
extern STEER_MOTOR steerMotor; // 舵机对象