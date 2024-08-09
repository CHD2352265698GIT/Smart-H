#include <DHT22.h>

unsigned char DHT22::ReadByte() // 读取一个字节
{
    unsigned int cnt = 0;       // 计数器
    char temp = 0;              // 临时变量
    unsigned char Data = 0x00;  // 数据变量
    for (int i = 0; i < 8; i++) // 8位数据
    {
        while (!READPIN) // 检测上次低电平是否结束
        {
            if (++cnt >= 300) // 超时
            {
                break; // 退出循环
            }
        }
        delayMicroseconds(2); // 延时30us
        temp = 0;
        if (READPIN) // 如果为高电平
        {
            temp = 1; // 临时变量赋值为1
        }
        cnt = 0;
        while (!READPIN) // 检测上次低电平是否结束
        {
            if (++cnt >= 200) // 超时
            {
                break; // 退出循环
            }
        }
        Data <<= 1;   // 左移一位
        Data |= temp; // 将临时变量赋值给Data
    }
    return Data; // 返回数据
}
char DHT22::readData() // 读取数据
{
    unsigned int cnt = 0;          // 计数器
    memset(Data, 0, sizeof(Data)); // 清空数据
    pinMode(pin, OUTPUT);          // 设置为输出模式
    digitalWrite(pin, LOW);        // 发送低电平
    delay(2);                      // 延时2ms
    digitalWrite(pin, HIGH);       // 发送高电平
    pinMode(pin, INPUT);           // 设置为输入模式
    delayMicroseconds(20);         // 延时30us
    if (!READPIN)                  // 如果为低电平
    {
        while (!READPIN) // 检测上次低电平是否结束
        {
            if (++cnt >= 300) // 超时
            {
                return 1;
            }
        }
        cnt = 0;        // 系统计数器赋值为0
        while (READPIN) // 检测上次高电平是否结束
        {
            if (++cnt >= 300) // 超时
            {
                return 2;
            }
        }
        for (int i = 0; i < 5; i++)
        {
            Data[i] = ReadByte();
        }
    }
    else
    {
        return 3;
    }
    if (Data[0] + Data[1] + Data[2] + Data[3] == Data[4]) // 检测数据是否正确
    {
        return 0;
    }
    else
    {
        return 4;
    }
}

void DHT22::printData() // 打印数据
{
    Serial.print("温度：");
    Serial.print(Data[2]);
    Serial.print(".");
    Serial.print(Data[3]);
    Serial.print(" 湿度：");
    Serial.print(Data[0]);
    Serial.print(".");
    Serial.print(Data[1]);
    Serial.println("%");
}
