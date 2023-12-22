#ifndef NFC_APPLE_APPLE_H_
#define NFC_APPLE_APPLE_H_

#include "../../protocol/protocol.h"
#include "../interface/interface.h"

#include <memory>

class Apple {
 public:
  enum class ReadResult { kOk = 0, kFail = 1 };

  Apple(std::shared_ptr<Protocol> protocol,
        std::shared_ptr<NFCInterface> interface)
      : protocol_(protocol), interface_(interface) {}

  ReadResult ReadAppleHomeKey();

 private:
  bool SelectHomeKeyAid(std::vector<uint8_t>& version_data);
  bool SelectAndVerifyVersion();

  bool FastAuth(const std::vector<uint8_t>& reader_ephemeral_apple_public_key,
                const std::vector<uint8_t>& transaction_id,
                const std::vector<uint8_t>& reader_id,
                std::vector<uint8_t>& endpoint_ephemeral_public_key_tag,
                std::vector<uint8_t>& returned_cryptogram);
  bool StandardAuth(const std::vector<uint8_t>& signature,
                    std::vector<uint8_t>& encrypted_response);
  bool SendControlFlow(uint8_t p1, uint8_t p2);

  std::shared_ptr<Protocol> protocol_;
  std::shared_ptr<NFCInterface> interface_;
};

#endif  // NFC_APPLE_APPLE_H_