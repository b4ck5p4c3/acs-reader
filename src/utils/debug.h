#ifndef UTILS_DEBUG_H_
#define UTILS_DEBUG_H_

#include <Arduino.h>
#include <vector>

#include "../mcu_config.h"

#ifdef DEBUG_ENABLED
#define DEBUG_PRINT(fmt, ...) DEBUG_SERIAL.printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ;
#endif

void InitDebug();
void HexDump(const char* preamble, const std::vector<uint8_t>& data);

#endif  // UTILS_DEBUG_H_