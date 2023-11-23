#ifndef UTILS_BEEPER_H_
#define UTILS_BEEPER_H_

#include <cstdint>
#include <vector>

void InitBeeper();
void Beep(const std::vector<uint32_t>& times);
void StartBeep();
void StopBeep();

#endif