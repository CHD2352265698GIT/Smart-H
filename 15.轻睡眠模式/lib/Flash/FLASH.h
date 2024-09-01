#pragma once
#include <Arduino.h>

// 定义页数量
#define FLASH_PAGE_NUM 16

// 定义wif信息所处页
#define WIFI_INFO_PAGE 0x6c
// 定义wif信息基地址
#define FLASH_BASE_ADDR 0x6c000
// 定义wifi账号长度地址
#define WIFI_SSID_LEN_ADDR FLASH_BASE_ADDR
// 定义wifi密码长度地址
#define WIFI_PWD_LEN_ADDR FLASH_BASE_ADDR + 0x01
// 定义Wifi信息存储地址
#define WIFI_INFO_ADDR FLASH_BASE_ADDR + 0x02

// 定义舵机所在页
#define WIFI_MOTOR_SECTOR_ADDR 0x6d
// 定义舵机转动角度地址
#define WIFI_MOTOR_ANGLE_ADDR 0x6d000

// 定义LogIn.html存储地址
#define LOGIN_HTML_ADDR 0x7c000
// 定义config.html存储地址
#define CONFIG_HTML_ADDR LOGIN_HTML_ADDR + 0x1000
// 定义LogIn.html大小
#define LOGIN_HTML_SIZE 2948 - 2
// 定义config.html大小
#define CONFIG_HTML_SIZE 4262 - 2