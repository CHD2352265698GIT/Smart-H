#pragma once

#include <Arduino.h>
#include <Servo.h>

extern Servo myservo;                 // 声明servo对象
#define STEER_MOTOR_MAX_ANGLE 180     // 舵机最大转动角度
#define STEER_MOTOR_MIN_ANGLE 0       // 舵机最小转动角度
#define STEER_MOTOR_DEFAULT_SPEED 100 // 舵机默认转动速度
#define STEER_MOTOR_PIN 2             // 舵机控制引脚

void Steer_LED();