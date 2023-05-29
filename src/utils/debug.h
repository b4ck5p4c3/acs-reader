#ifndef UTILS_DEBUG_H_
#define UTILS_DEBUG_H_

#include <Arduino.h>

#include "../mcu_config.h"

#ifdef DEBUG_ENABLED
#define DEBUG_PRINT(fmt, ...) DEBUG_SERIAL.printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ;
#endif

void InitDebug();

#endif