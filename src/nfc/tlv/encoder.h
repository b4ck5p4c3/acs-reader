#ifndef NFC_TLV_ENCODER_H_
#define NFC_TLV_ENCODER_H_

#include <cstdint>
#include <vector>

struct TLVData {
  const std::vector<std::pair<uint32_t, TLVData>> entries;
  const std::vector<uint8_t> data;
};

std::vector<uint8_t> EncodeTLV(const TLVData &data);

#endif  // NFC_TLV_ENCODER_H_