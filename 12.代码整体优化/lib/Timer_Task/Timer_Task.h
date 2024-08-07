#pragma once

class Timer_Task
{
public:
    Timer_Task(unsigned int Settime);     // 构造函数，设置任务间隔运行时间(该计数时间为定时器进入中断次数)
                                          // 如：定时器进入中断时间设置为1秒，Settime=2，则每两秒运行一次客户任务
    void Run();                           // 定时器中断任务，用于计数
    void RunTask(void (*callback)(void)); // 客户任务函数
private:
    unsigned int Settime; // 任务间隔时间
    unsigned int count;   // 计数器
    unsigned char flag;   // 任务标志
};
