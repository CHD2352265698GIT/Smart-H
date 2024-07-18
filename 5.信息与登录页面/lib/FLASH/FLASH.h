#pragma once
#include <Arduino.h>
// 定义所处页
#define FLASH_PAGE 0x6c
// 定义基地址
#define FLASH_BASE_ADDR 0x6c000
// 定义页数量
#define FLASH_PAGE_NUM 16
// 定义Wifi信息存储地址
#define WIFI_INFO_ADDR FLASH_BASE_ADDR + 0x02
// 定义wifi账号长度地址
#define WIFI_SSID_LEN_ADDR FLASH_BASE_ADDR + 0x00
// 定义wifi密码长度地址
#define WIFI_PWD_LEN_ADDR FLASH_BASE_ADDR + 0x01
// 定义index.html存储地址
#define INDEX_HTML_ADDR FLASH_BASE_ADDR + 0x10000
// 定义LogIn.html存储地址
#define LOGIN_HTML_ADDR INDEX_HTML_ADDR + INDEX_HTML_SIZE
// 定义config.html存储地址
#define CONFIG_HTML_ADDR LOGIN_HTML_ADDR + LOGIN_HTML_SIZE
// 定义index.html大小
#define INDEX_HTML_SIZE 3161
// 定义LogIn.html大小
#define LOGIN_HTML_SIZE 3161 - 1
// 定义config.html大小
#define CONFIG_HTML_SIZE 3161 - 1