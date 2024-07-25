#include <Arduino.h>
#include "Timer_Task.h"

Timer_Task::Timer_Task(unsigned int Settime)
{
    if (Settime > 65534)
    {
        Settime = 65534;
        Serial.println("Settime is too large, set to 65534.");
    }
    this->Settime = Settime;
}
void Timer_Task::Run()
{
    if (count == Settime)
    {
        flag = 1;
    }
    else
        count++;
}
void Timer_Task::RunTask(void (*callback)(void))
{
    if (flag == 1)
    {
        callback();
        flag = 0;
        count = 0;
    }
}
