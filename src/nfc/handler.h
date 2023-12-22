#ifndef NFC_HANDLER_H_
#define NFC_HANDLER_H_

#include "../protocol/protocol.h"
#include "apple/apple.h"
#include "emv/emv.h"
#include "interface/interface.h"
#include "../utils/beeper.h"

#include <memory>

class NFC {
 public:
  NFC(std::shared_ptr<Protocol> protocol,
      std::shared_ptr<NFCInterface> interface, std::shared_ptr<Beeper> beeper)
      : protocol_(protocol),
        interface_(interface),
        beeper_(beeper),
        emv_(interface),
        apple_(protocol, interface) {}

  void Handle();

 private:
  enum class ReadResult { kNoTag = 0, kTimedOut = 1, kTag = 2 };

  ReadResult ReadAndClassifyTarget(std::vector<uint8_t>& uid);

  std::shared_ptr<Protocol> protocol_;
  std::shared_ptr<NFCInterface> interface_;
  std::shared_ptr<Beeper> beeper_;
  EMV emv_;
  Apple apple_;
};

#endif  // NFC_HANDLER_H_