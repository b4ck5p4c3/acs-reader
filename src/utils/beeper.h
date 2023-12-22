#ifndef UTILS_BEEPER_H_
#define UTILS_BEEPER_H_

#include <cstdint>
#include <vector>

class Beeper {
 public:
  Beeper(uint32_t vcc_pin, uint32_t gnd_pin)
      : vcc_pin_(vcc_pin), gnd_pin_(gnd_pin) {}

  void Init();
  void Beep(const std::vector<uint32_t>& times);
  void StartBeep();
  void StopBeep();

 private:
  uint32_t vcc_pin_;
  uint32_t gnd_pin_;
};

#endif  // UTILS_BEEPER_H_