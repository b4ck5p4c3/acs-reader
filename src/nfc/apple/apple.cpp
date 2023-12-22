#include "apple.h"
#include "../../utils/debug.h"
#include "../tlv/encoder.h"
#include "../tlv/parser.h"

#include <vector>

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/md.h>
#include <mbedtls/pk.h>

const std::vector<uint8_t> kAppleHomeKeyAid{0xA0, 0x00, 0x00, 0x08,
                                            0x58, 0x01, 0x01};

const uint16_t kAppleHomeKeyVersion = 0x0200;

bool Apple::SelectHomeKeyAid(std::vector<uint8_t>& version_data) {
  std::vector<uint8_t> ber;

  // Select AID
  // Header: 0x00 0xA4 0x04 0x00, Data: AID
  // AID: 0xA0 0x00 0x00 0x08 0x58 0x01 0x01
  std::vector<uint8_t> apdu{0x00, 0xA4, 0x04, 0x00,
                            static_cast<uint8_t>(kAppleHomeKeyAid.size())};

  apdu.insert(apdu.end(), kAppleHomeKeyAid.begin(), kAppleHomeKeyAid.end());
  apdu.push_back(0);

  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  HexDump("AHK_SELECT_PACKET", ber);

  // Remove APDU response status
  ber.pop_back();
  ber.pop_back();

  Parser tlv_parser(ber);

  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  Parser version_tlv_data = tlv_parser.GetObject(0x5C);
  if (version_tlv_data.IsNull()) {
    DEBUG_PRINT("Failed 0x5C\n");
    return false;
  }

  const std::vector<uint8_t> version_raw_data = version_tlv_data.GetData();
  version_data.clear();
  version_data.insert(version_data.begin(), version_raw_data.begin(),
                      version_raw_data.end());
  return true;
}

bool GenerateEphemeralKey(mbedtls_pk_context& key) {
  mbedtls_pk_init(&key);

  if (mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY))) {
    DEBUG_PRINT("PK setup failed\n");
    mbedtls_pk_free(&key);
    return false;
  }

  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  if (mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, nullptr,
                            0)) {
    DEBUG_PRINT("CTR DRBG seed failed\n");
    mbedtls_pk_free(&key);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    return false;
  }

  mbedtls_entropy_free(&entropy);

  if (mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(key),
                          mbedtls_ctr_drbg_random, &ctr_drbg)) {
    DEBUG_PRINT("ECP key generation failed\n");
    mbedtls_pk_free(&key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    return false;
  }

  mbedtls_ctr_drbg_free(&ctr_drbg);

  return true;
}

bool GetApplePublicKey(mbedtls_ecp_keypair* keypair,
                       std::vector<uint8_t>& result) {
  std::vector<uint8_t> x(256);
  x.resize(mbedtls_mpi_size(&keypair->Q.X));
  if (mbedtls_mpi_write_binary(&keypair->Q.X, x.data(), x.size())) {
    DEBUG_PRINT("MPI write binary failed for X\n");
    return false;
  }

  std::vector<uint8_t> y(256);
  y.resize(mbedtls_mpi_size(&keypair->Q.Y));
  if (mbedtls_mpi_write_binary(&keypair->Q.Y, y.data(), y.size())) {
    DEBUG_PRINT("MPI write binary failed for Y\n");
    return false;
  }

  result.clear();
  result.push_back(0x04);
  result.insert(result.end(), x.begin(), x.end());
  result.insert(result.end(), y.begin(), y.end());
  return true;
}

bool Apple::FastAuth(
    const std::vector<uint8_t>& reader_ephemeral_apple_public_key,
    const std::vector<uint8_t>& transaction_id,
    const std::vector<uint8_t>& reader_id,
    std::vector<uint8_t>& endpoint_ephemeral_public_key_tag,
    std::vector<uint8_t>& returned_cryptogram) {
  const std::vector<uint8_t> tlv_data =
      EncodeTLV({{{0x5C,
                   {{},
                    {static_cast<uint8_t>(kAppleHomeKeyVersion >> 8),
                     static_cast<uint8_t>(kAppleHomeKeyVersion & 0xFF)}}},
                  {0x87, {{}, reader_ephemeral_apple_public_key}},
                  {0x4C, {{}, transaction_id}},
                  {0x4D, {{}, reader_id}}},
                 {}});

  std::vector<uint8_t> ber;

  // Send AUTH0
  // Header: 0x80 0x80 0x01 0x01 len, Data: TLV
  std::vector<uint8_t> apdu{0x80, 0x80, 0x01, 0x01,
                            static_cast<uint8_t>(tlv_data.size())};

  apdu.insert(apdu.end(), tlv_data.begin(), tlv_data.end());

  HexDump("AUTH0_REQUEST", apdu);

  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  HexDump("AUTH0_RESPONSE", ber);

  ber.pop_back();
  ber.pop_back();

  Parser tlv_parser(ber);
  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  Parser endpoint_ephemeral_public_key_tag_tlv_data =
      tlv_parser.GetObject(0x86);
  if (endpoint_ephemeral_public_key_tag_tlv_data.IsNull()) {
    DEBUG_PRINT("Failed 0x86\n");
    return false;
  }

  Parser returned_cryptogram_tlv_data = tlv_parser.GetObject(0x9D);
  if (returned_cryptogram_tlv_data.IsNull()) {
    DEBUG_PRINT("Failed 0x9D\n");
    return false;
  }

  const std::vector<uint8_t> endpoint_ephemeral_public_key_tag_raw_data =
      endpoint_ephemeral_public_key_tag_tlv_data.GetData();
  endpoint_ephemeral_public_key_tag.clear();
  endpoint_ephemeral_public_key_tag.insert(
      endpoint_ephemeral_public_key_tag.begin(),
      endpoint_ephemeral_public_key_tag_raw_data.begin(),
      endpoint_ephemeral_public_key_tag_raw_data.end());

  const std::vector<uint8_t> returned_cryptogram_raw_data =
      returned_cryptogram_tlv_data.GetData();
  returned_cryptogram.clear();
  returned_cryptogram.insert(returned_cryptogram.begin(),
                             returned_cryptogram_raw_data.begin(),
                             returned_cryptogram_raw_data.end());

  return true;
}

bool Apple::SelectAndVerifyVersion() {
  std::vector<uint8_t> version_data;
  if (!this->SelectHomeKeyAid(version_data)) {
    return false;
  }

  HexDump("AHK_VERSION", version_data);
  if (version_data.size() % 2 != 0) {
    return false;
  }

  bool supported = false;

  for (size_t i = 0; i < version_data.size(); i += 2) {
    uint16_t version = (version_data[i] << 8u) | version_data[i + 1];
    DEBUG_PRINT("Returned version: %04x\n", version);
    if (version == kAppleHomeKeyVersion) {
      supported = true;
    }
  }

  DEBUG_PRINT("Supported: %d\n", supported);

  return supported;
}

bool Apple::SendControlFlow(uint8_t p1, uint8_t p2) {
  // Send OP_CONTROL_FLOW
  // Header: 0x80 0x80 0x01 0x01 len, Data: TLV
  std::vector<uint8_t> ber;
  std::vector<uint8_t> apdu{0x80, 0x3C, p1, p2};

  HexDump("OP_CONTROL_FLOW_REQUEST", apdu);
  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
    return false;
  }
  HexDump("OP_CONTROL_FLOW_RESPONSE", ber);

  return true;
}

bool Apple::StandardAuth(const std::vector<uint8_t>& signature,
                    std::vector<uint8_t>& encrypted_response) {
  const std::vector<uint8_t> tlv_data =
      EncodeTLV({{{0x9E, {{}, signature}}},
                 {}});

  std::vector<uint8_t> apdu{0x80, 0x81, 0x00, 0x00, static_cast<uint8_t>(tlv_data.size())};
  apdu.insert(apdu.end(), tlv_data.begin(), tlv_data.end());
  // Send AUTH1
  // Header: 0x80 0x81 0x00 0x00 len, Data: TLV

  HexDump("AUTH1_REQUEST", apdu);
  if (!this->interface_->ApduExchange(apdu, encrypted_response, 1000)) {
    return false;
  }
  
  HexDump("AUTH1_RESPONSE", encrypted_response);
  encrypted_response.pop_back();
  encrypted_response.pop_back();

  if (encrypted_response.size() == 0) {
    return false;
  }

  return true;
}

Apple::ReadResult Apple::ReadAppleHomeKey() {
  if (!this->SelectAndVerifyVersion()) {
    return Apple::ReadResult::kFail;
  }

  std::vector<uint8_t> reader_id;
  std::vector<uint8_t> transaction_id;
  std::vector<uint8_t> reader_ephemeral_apple_public_key;

  if (!this->protocol_->SendAppleTransactionRequest(reader_id,
                                                    transaction_id,
                                                    reader_ephemeral_apple_public_key)) {
    return Apple::ReadResult::kFail;
  }

  HexDump("TRANSACTION_ID", transaction_id);
  HexDump("READER_ID", reader_id);
  HexDump("READER_PUBLIC_KEY", reader_ephemeral_apple_public_key);

  std::vector<uint8_t> endpoint_ephemeral_public_key_tag;
  std::vector<uint8_t> returned_cryptogram;
  if (!this->FastAuth(reader_ephemeral_apple_public_key, transaction_id,
                      reader_id, endpoint_ephemeral_public_key_tag,
                      returned_cryptogram)) {
    return Apple::ReadResult::kFail;
  }

  HexDump("ENDPOINT_EPHEMERAL_PUBLIC_KEY_TAG",
          endpoint_ephemeral_public_key_tag);
  HexDump("RETURNED_CRYPTOGRAM", returned_cryptogram);

  bool fast_auth_success;
  std::vector<uint8_t> signature;

  if (!this->protocol_->SendAppleCryptogramVerifyRequest(
          reader_ephemeral_apple_public_key, endpoint_ephemeral_public_key_tag,
          returned_cryptogram, fast_auth_success, signature)) {
    return Apple::ReadResult::kFail;
  }

  if (fast_auth_success) {
    if (!this->SendControlFlow(0x01, 0x00)) {
      return Apple::ReadResult::kFail;
    }
    return Apple::ReadResult::kOk;
  }

  std::vector<uint8_t> encrypted_response;

  if (!this->StandardAuth(signature, encrypted_response)) {
    return Apple::ReadResult::kFail;
  }

  return Apple::ReadResult::kFail;
}