#include <Arduino.h>

/* 点灯工程 */

// put function declarations here:
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200); // 设置串口波特率
  pinMode(2, OUTPUT);   // 设置GPIO模式
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("hello world"); // 串口打印字符串
  digitalWrite(2, LOW);          // 设置GPIO状态
  delay(500);
  digitalWrite(2, HIGH); // 设置GPIO状态
  delay(500);
}

// put function definitions here:
