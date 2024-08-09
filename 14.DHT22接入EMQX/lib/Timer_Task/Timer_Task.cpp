#include <Arduino.h>
#include "Timer_Task.h"

Timer_Task::Timer_Task(unsigned int Settime, Set flag) // 构造函数，设置任务间隔运行时间
{
    if (Settime > 65534) // 检查Settime是否大于65534
    {
        Settime = 65534;                                                // 如果大于，则将其设置为65534
        Serial.println("Warring: Settime is too large, set to 65534."); // 打印警告信息
    }
    this->flag = flag;       // 设置标志位
    this->Settime = Settime; // 设置定时器时间
}
void Timer_Task::Run() // 定时器中断任务
{
    if (count == Settime) // 如果计数器等于定时器时间
    {
        flag = 1; // 设置标志位为1
    }
    else
        count++; // 否则计数器加1
}
void Timer_Task::RunTask(void (*callback)(void)) // 客户任务
{
    if (flag == 1) // 如果标志位为1
    {
        callback(); // 执行回调函数
        flag = 0;   // 清除标志位
        count = 0;  // 清除计数器
    }
}
