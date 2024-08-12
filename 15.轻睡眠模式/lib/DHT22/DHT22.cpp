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
        delayMicroseconds(30); // 延时30us
        temp = 0;
        if (READPIN) // 如果为高电平
        {
            temp = 1; // 临时变量赋值为1
        }
        cnt = 0;
        while (READPIN) // 检测上次低电平是否结束
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
        while (!READPIN) // 检测低电平是否结束
        {
            if (++cnt >= 300) // 超时
            {
                errorNumber = 1;
                return errorNumber;
            }
        }
        cnt = 0;        // 系统计数器赋值为0
        while (READPIN) // 检测高电平是否结束
        {
            if (++cnt >= 300) // 超时
            {
                errorNumber = 2;
                return errorNumber;
            }
        }
        for (int i = 0; i < 5; i++) // 读取5个字节
        {
            Data[i] = ReadByte(); // 读取数据
        }
    }
    else
    {
        errorNumber = 3; // DHT11未连接
        return errorNumber;
    }
    if (Data[0] + Data[1] + Data[2] + Data[3] == Data[4]) // 检测数据是否正确
    {
        symbol = Data[2] & 0x80; // 获取符号位
        Data[2] &= 0x7f;         // 去掉符号位
        if (symbol)              // 如果为负数
        {
            temperature = (Data[2] * 256 + Data[3]) * -1; // 转换为负数
        }
        else
        {
            temperature = Data[2] * 256 + Data[3]; // 转换为正数
        }
        humidity = Data[0] * 256 + Data[1]; // 转换为湿度
        temperature /= 10.0;                // 转换为浮点数
        humidity /= 10.0;                   // 转换为浮点数
        errorNumber = 0;
        return errorNumber;
    }
    else
    {
        errorNumber = 4; // 数据校验错误
        return errorNumber;
    }
}

const char *DHT22::ErrorData() // 打印数据
{
    switch (errorNumber)
    {
    case 1:
        return "Error: Low level not ended"; // 低电平未结束
    case 2:
        return "Error: High level not ended"; // 高电平未结束
    case 3:
        return "Error: DHT11 not connected"; // DHT11未连接
    case 4:
        return "Error: Data verification failed"; // 数据校验错误
    default:
        return "Error: Unknown error"; // 未知错误
    }
}

float DHT22::getHumidity() // 获取湿度
{
    return humidity;
}

float DHT22::getTemperature() // 获取温度
{
    return temperature;
}