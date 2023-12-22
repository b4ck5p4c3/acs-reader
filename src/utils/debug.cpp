#include "debug.h"

void InitDebug() {
#ifdef DEBUG_ENABLED
  DEBUG_SERIAL.begin(115200);
#endif
}

void HexDump(const char* preamble, const std::vector<uint8_t>& data) {
  DEBUG_PRINT("%s: ", preamble);
  for (auto byte : data) {
    DEBUG_PRINT("%02x", byte);
  }
  DEBUG_PRINT("\n");
}