#ifndef UTILS_OUTPUT_H_
#define UTILS_OUTPUT_H_

#include <Arduino.h>

#include "../mcu_config.h"

void InitOutput();
void OutputReadID(const char* type, const char* data);

#endif