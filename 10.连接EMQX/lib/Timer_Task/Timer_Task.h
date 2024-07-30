#pragma once

class Timer_Task
{
public:
    Timer_Task(unsigned int Settime);
    void Run();
    void RunTask(void (*callback)(void));

private:
    void (*callback)(void);
    unsigned int Settime;
    unsigned int count;
    unsigned char flag;
};
