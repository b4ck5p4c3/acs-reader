#ifndef NFC_INTERFACE_INTERFACE_H_
#define NFC_INTERFACE_INTERFACE_H_

#include <cstdint>
#include <vector>

struct NFCTagInfo {
  uint8_t sak;
  uint32_t atqa;
  std::vector<uint8_t> uid;
  std::vector<uint8_t> ats;
};

class NFCInterface {
 public:
  NFCInterface(const NFCInterface& other) = default;
  NFCInterface(NFCInterface&& other) = default;
  NFCInterface& operator=(const NFCInterface& other) = default;
  NFCInterface& operator=(NFCInterface&& other) = default;

  virtual bool Init() = 0;
  virtual bool FindTag(NFCTagInfo& info, uint32_t timeout) = 0;
  virtual bool ApduExchange(const std::vector<uint8_t>& in_data,
                            std::vector<uint8_t>& out_data,
                            uint32_t timeout) = 0;

 protected:
  NFCInterface() = default;
};

#endif