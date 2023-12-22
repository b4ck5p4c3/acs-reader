#ifndef PROTOCOL_PROTOCOL_H_
#define PROTOCOL_PROTOCOL_H_

#include <Stream.h>
#include <string>
#include <vector>

class Protocol {
 public:
  Protocol(Stream& stream) : stream_(stream) {}

  void SendRfid125KHzUidRecieved(const std::string& uid);
  void SendNfcUIDRecieved(const std::string& uid);
  void SendPANRecievedNotify(const std::string& pan);
  bool SendAppleTransactionRequest(
      std::vector<uint8_t>& reader_id, std::vector<uint8_t>& transaction_id,
      std::vector<uint8_t>& reader_ephemeral_public_key);
  bool SendAppleCryptogramVerifyRequest(
      const std::vector<uint8_t>& reader_ephemeral_public_key,
      const std::vector<uint8_t>& endpoint_ephemeral_public_key,
      const std::vector<uint8_t>& cryptogram, bool& success,
      std::vector<uint8_t>& signature);
  bool SendAppleSignatureVerifyRequest(const std::vector<uint8_t>& signature,
                                       const std::vector<uint8_t>& device_id,
                                       bool& success);

 private:
  void SendPacket(uint8_t id, const std::vector<uint8_t>& data);
  bool RecievePacket(uint8_t& id, std::vector<uint8_t>& data);
  bool ReadFully(std::vector<uint8_t>& data);

  Stream& stream_;
};

#endif  // PROTOCOL_PROTOCOL_H_
