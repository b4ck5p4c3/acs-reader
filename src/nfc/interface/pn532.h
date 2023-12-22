#ifndef NFC_INTERFACE_PN532_H_
#define NFC_INTERFACE_PN532_H_

#include "interface.h"

#include <Wire.h>

class PN532 : public NFCInterface {
 public:
  PN532(TwoWire wire, uint32_t scl_pin, uint32_t sda_pin, uint32_t irq_pin,
        uint32_t rst_pin)
      : wire_(wire),
        scl_pin_(scl_pin),
        sda_pin_(sda_pin),
        irq_pin_(irq_pin),
        rst_pin_(rst_pin) {}

  virtual bool Init();
  virtual bool FindTag(NFCTagInfo& info, uint32_t timeout);
  virtual bool ApduExchange(const std::vector<uint8_t>& in_data,
                            std::vector<uint8_t>& out_data, uint32_t timeout);

  bool GetFirmwareVersion(uint32_t& version);

  bool SetPassiveActivationRetries(uint8_t max_retries);
  bool SAMConfigure();

 private:
  bool CommandExchange(const std::vector<uint8_t>& in_data,
                       std::vector<uint8_t>& out_data, uint32_t timeout = 1000);
  bool WaitReady(uint32_t timeout);
  bool SendCommandData(const std::vector<uint8_t>& data);
  bool ReadAck(uint32_t timeout);
  bool ReadRdy();
  bool ReadResponse(std::vector<uint8_t>& data, uint32_t timeout);

  TwoWire wire_;
  uint32_t scl_pin_;
  uint32_t sda_pin_;
  uint32_t irq_pin_;
  uint32_t rst_pin_;
};

#endif  // NFC_INTERFACE_PN532_H_