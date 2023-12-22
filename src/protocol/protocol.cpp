#include "protocol.h"
#include "../utils/debug.h"
#include "compiled/messages.h"

#include <esp32/rom/crc.h>

#define MAX_BUFFER_LENGTH 256

const std::vector<uint8_t> kPreamble{0xDE, 0xAD, 0xBE, 0xEF,
                                     0xCA, 0xFE, 0xBA, 0xBE};

class VectorWriteBufferInterface
    : public ::EmbeddedProto::WriteBufferInterface {
 public:
  VectorWriteBufferInterface() = default;

  virtual void clear() { this->buffer.clear(); }

  virtual uint32_t get_size() const { return this->buffer.size(); }

  virtual uint32_t get_max_size() const { return MAX_BUFFER_LENGTH * 128; }

  virtual uint32_t get_available_size() const {
    return get_max_size() - get_size();
  }

  virtual bool push(uint8_t byte) {
    this->buffer.push_back(byte);
    return true;
  }

  virtual bool push(const uint8_t* bytes, const uint32_t length) {
    this->buffer.insert(this->buffer.end(), bytes, bytes + length);
    return true;
  }

  virtual ~VectorWriteBufferInterface() = default;

  std::vector<uint8_t> GetBuffer() { return this->buffer; }

 private:
  std::vector<uint8_t> buffer;
};

class VectorReadBufferInterface : public ::EmbeddedProto::ReadBufferInterface {
 public:
  VectorReadBufferInterface(const std::vector<uint8_t>& data)
      : data_(data), index_(0) {}

  virtual ~VectorReadBufferInterface() = default;

  virtual uint32_t get_size() const { return this->data_.size(); }

  virtual uint32_t get_max_size() const { return this->data_.size(); }

  virtual bool peek(uint8_t& byte) const {
    if (this->index_ >= this->data_.size()) {
      return false;
    }
    byte = this->data_[index_];
    return true;
  }

  virtual bool advance() {
    if ((this->index_ + 1) >= this->data_.size()) {
      return false;
    }
    this->index_++;
    return true;
  }

  virtual bool advance(const uint32_t n_bytes) {
    if ((this->index_ + n_bytes) >= this->data_.size()) {
      return false;
    }
    this->index_ += n_bytes;
    return true;
  }

  virtual bool pop(uint8_t& byte) {
    if (this->index_ >= this->data_.size()) {
      return false;
    }
    byte = this->data_[this->index_++];
    return true;
  }

 private:
  std::vector<uint8_t> data_;
  uint32_t index_;
};

void Protocol::SendRfid125KHzUidRecieved(const std::string& uid) {
  Rfid125KHzUidRecievedNotify<MAX_BUFFER_LENGTH> message;
  message.mutable_uid().set(uid.c_str(), uid.size());
  VectorWriteBufferInterface interface;
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(Rfid125KHzUidRecievedNotify<
                           MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID),
      interface.GetBuffer());
}

void Protocol::SendNfcUIDRecieved(const std::string& uid) {
  NfcUIDRecievedNotify<MAX_BUFFER_LENGTH> message;
  message.mutable_uid().set(uid.c_str(), uid.size());
  VectorWriteBufferInterface interface;
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(
          NfcUIDRecievedNotify<MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID),
      interface.GetBuffer());
}

void Protocol::SendPANRecievedNotify(const std::string& pan) {
  PANRecievedNotify<MAX_BUFFER_LENGTH> message;
  message.mutable_pan().set(pan.c_str(), pan.size());
  VectorWriteBufferInterface interface;
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(
          PANRecievedNotify<MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID),
      interface.GetBuffer());
}

bool Protocol::SendAppleTransactionRequest(
    std::vector<uint8_t>& reader_id, std::vector<uint8_t>& transaction_id,
    std::vector<uint8_t>& reader_ephemeral_public_key) {
  AppleTransactionRequest message;
  VectorWriteBufferInterface interface;
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(AppleTransactionRequest::PacketInfo::PACKET_ID),
      interface.GetBuffer());
  std::vector<uint8_t> data;
  uint8_t id;
  bool success = this->RecievePacket(id, data);
  if (!success ||
      id != static_cast<uint8_t>(AppleTransactionResponse<
                                 MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH,
                                 MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID)) {
    return false;
  }

  AppleTransactionResponse<MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH,
                           MAX_BUFFER_LENGTH>
      response;
  VectorReadBufferInterface responseInterface(data);
  response.deserialize(responseInterface);
  reader_id.clear();
  reader_id.insert(reader_id.begin(), response.get_reader_id().get_const(),
                   response.get_reader_id().get_const() +
                       response.get_reader_id().get_length());
  transaction_id.clear();
  transaction_id.insert(transaction_id.begin(),
                        response.get_transaction_id().get_const(),
                        response.get_transaction_id().get_const() +
                            response.get_transaction_id().get_length());
  reader_ephemeral_public_key.clear();
  reader_ephemeral_public_key.insert(
      reader_ephemeral_public_key.begin(),
      response.get_reader_ephemeral_public_key().get_const(),
      response.get_reader_ephemeral_public_key().get_const() +
          response.get_reader_ephemeral_public_key().get_length());
  return true;
}

bool Protocol::SendAppleCryptogramVerifyRequest(
    const std::vector<uint8_t>& reader_ephemeral_public_key,
    const std::vector<uint8_t>& endpoint_ephemeral_public_key,
    const std::vector<uint8_t>& cryptogram, bool& success,
    std::vector<uint8_t>& signature) {
  AppleCryptogramVerifyRequest<MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH,
                               MAX_BUFFER_LENGTH>
      message;
  VectorWriteBufferInterface interface;
  message.mutable_reader_ephemeral_public_key().set(
      reader_ephemeral_public_key.data(), reader_ephemeral_public_key.size());
  message.mutable_endpoint_ephemeral_public_key().set(
      endpoint_ephemeral_public_key.data(),
      endpoint_ephemeral_public_key.size());
  message.mutable_cryptogram().set(cryptogram.data(), cryptogram.size());
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(AppleCryptogramVerifyRequest<
                           MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH,
                           MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID),
      interface.GetBuffer());
  std::vector<uint8_t> data;
  uint8_t id;
  bool recieved = this->RecievePacket(id, data);
  if (!recieved ||
      id != static_cast<uint8_t>(AppleCryptogramVerifyResponse<
                                 MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID)) {
    return false;
  }

  AppleCryptogramVerifyResponse<MAX_BUFFER_LENGTH> response;
  VectorReadBufferInterface responseInterface(data);
  response.deserialize(responseInterface);
  success = response.get_success();
  signature.clear();
  signature.insert(signature.begin(), response.get_signature().get_const(),
                   response.get_signature().get_const() +
                       response.get_signature().get_length());
  return true;
}

bool Protocol::SendAppleSignatureVerifyRequest(
    const std::vector<uint8_t>& signature,
    const std::vector<uint8_t>& device_id, bool& success) {
  AppleSignatureVerifyRequest<MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH> message;
  VectorWriteBufferInterface interface;
  message.mutable_signature().set(signature.data(), signature.size());
  message.mutable_device_id().set(device_id.data(), device_id.size());
  message.serialize(interface);
  this->SendPacket(
      static_cast<uint8_t>(
          AppleSignatureVerifyRequest<
              MAX_BUFFER_LENGTH, MAX_BUFFER_LENGTH>::PacketInfo::PACKET_ID),
      interface.GetBuffer());
  std::vector<uint8_t> data;
  uint8_t id;
  bool recieved = this->RecievePacket(id, data);
  if (!recieved ||
      id != static_cast<uint8_t>(
                AppleSignatureVerifyResponse::PacketInfo::PACKET_ID)) {
    return false;
  }

  AppleSignatureVerifyResponse response;
  VectorReadBufferInterface responseInterface(data);
  response.deserialize(responseInterface);
  success = response.get_success();
  return true;
}

void Protocol::SendPacket(uint8_t id, const std::vector<uint8_t>& data) {
  if (data.size() > 65535) {
    return;
  }
  std::vector<uint8_t> result_packet;
  result_packet.insert(result_packet.end(), kPreamble.begin(), kPreamble.end());
  result_packet.push_back(static_cast<uint8_t>(data.size() >> 8));
  result_packet.push_back(static_cast<uint8_t>(data.size() & 0xFF));
  result_packet.push_back(id);
  result_packet.insert(result_packet.end(), data.begin(), data.end());
  uint32_t crc = crc32_le(0, result_packet.data(), result_packet.size());
  result_packet.push_back(crc >> 24);
  result_packet.push_back((crc >> 16) & 0xFF);
  result_packet.push_back((crc >> 8) & 0xFF);
  result_packet.push_back(crc & 0xFF);
  size_t written = 0;
  while (written < result_packet.size()) {
    written += this->stream_.write(result_packet.data() + written,
                                   result_packet.size() - written);
  }
}

bool Protocol::ReadFully(std::vector<uint8_t>& data) {
  size_t read = 0;
  while (read < data.size()) {
    size_t current_read =
        this->stream_.readBytes(data.data() + read, data.size() - read);
    if (current_read == 0) {
      return false;
    }
    read += current_read;
  }
  return true;
}

bool Protocol::RecievePacket(uint8_t& id, std::vector<uint8_t>& data) {
  std::vector<uint8_t> preamble(8);
  if (!this->ReadFully(preamble)) {
    return false;
  }

  while (preamble != kPreamble) {
    preamble.erase(preamble.begin());
    std::vector<uint8_t> preamble_part(1);
    if (!this->ReadFully(preamble_part)) {
      return false;
    }
    preamble.push_back(preamble_part[0]);
  }

  std::vector<uint8_t> lengthAndId(3);
  if (!this->ReadFully(lengthAndId)) {
    return false;
  }
  id = lengthAndId[2];
  uint16_t real_length = (lengthAndId[0] << 8u) | lengthAndId[1];
  data.resize(real_length);
  if (!this->ReadFully(data)) {
    return false;
  }
  std::vector<uint8_t> crc(4);
  if (!this->ReadFully(crc)) {
    return false;
  }
  std::vector<uint8_t> data_to_crc;
  data_to_crc.insert(data_to_crc.end(), preamble.begin(), preamble.end());
  data_to_crc.insert(data_to_crc.end(), lengthAndId.begin(), lengthAndId.end());
  data_to_crc.insert(data_to_crc.end(), data.begin(), data.end());
  uint32_t crc_calculated = crc32_le(0, data_to_crc.data(), data_to_crc.size());
  uint32_t crc_received =
      (crc[0] << 24u) | (crc[1] << 16u) | (crc[2] << 8u) | crc[3];
  if (crc_calculated != crc_received) {
    DEBUG_PRINT("CRC %04x != %04x\n", crc_calculated, crc_received);
    return false;
  }
  return true;
}