#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <LED.h>
#define STEER_MOTOR_MAX_ANGLE 180     // 舵机最大转动角度
#define STEER_MOTOR_MIN_ANGLE 0       // 舵机最小转动角度
#define STEER_MOTOR_DEFAULT_SPEED 100 // 舵机默认转动速度
#define STEER_MOTOR_PIN 2             // 舵机控制引脚

class STEER_MOTOR : public Servo
{
public:
    STEER_MOTOR(int pin = STEER_MOTOR_PIN) // 构造函数
    {
        this->pin = pin;
    }
    void setAngle(int angle); // 设置舵机转动角度
    // 设置舵机开关灯
    void setLed(char on);
    void setOnValue(char value) // 设置舵机开灯值
    {
        OnValue = value;
    }
    void setOffValue(char value) // 设置舵机关灯值
    {
        OffValue = value;
    }

private:
    char OnValue;  // 舵机开灯值
    char OffValue; // 舵机关灯值
    int pin;       // 舵机控制引脚
};
extern STEER_MOTOR steerMotor; // 舵机对象