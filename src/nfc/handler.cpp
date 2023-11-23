#include "handler.h"
#include "../mcu_config.h"
#include "../utils/beeper.h"
#include "../utils/debug.h"
#include "../utils/output.h"
#include "interface/pn532.h"
#include "tlv/parser.h"

#include <iomanip>
#include <sstream>

#define RFID_READ_NO_TAG 0x00
#define RFID_READ_TIMED_OUT 0x01
#define RFID_READ_UID 0x10
#define RFID_READ_EMVCO 0x11

#define EMVCO_READ_OK 0x00
#define EMVCO_READ_FAIL 0x10

PN532 nfc(Wire, PN532_SCL, PN532_SDA, PN532_IRQ, PN532_RST);

void InitNFC() {
  if (!nfc.Init()) {
    DEBUG_PRINT("PN532 Connection Failed\n");
    while (1)
      ;  // halt
  }
}

uint8_t ReadAndClassifyTarget(std::vector<uint8_t> uid) {
  NFCTagInfo info;
  if (!nfc.FindTag(info, 10000)) {
    return RFID_READ_TIMED_OUT;
  }

  uid.clear();
  uid.insert(uid.begin(), info.uid.begin(), info.uid.end());

  return RFID_READ_EMVCO;
}

void HexDump(const char* preamble, const std::vector<uint8_t>& data) {
  DEBUG_PRINT("%s: ", preamble);
  for (auto byte : data) {
    DEBUG_PRINT("%02x", byte);
  }
  DEBUG_PRINT("\n");
}

void OutputHexData(const char* type, const std::vector<uint8_t>& data) {
  std::ostringstream output_stream;
  output_stream << std::uppercase << std::setfill('0') << std::setw(2)
                << std::right << std::hex;
  for (auto byte : data) {
    output_stream << byte;
  }
  OutputReadID(type, output_stream.str().c_str());
}

void OutputPan(const char* type, const std::vector<uint8_t>& data) {
  std::ostringstream output_stream;
  for (auto byte : data) {
    output_stream << static_cast<char>(byte);
  }
  OutputReadID(type, output_stream.str().c_str());
}

bool EMVGetAID(std::vector<uint8_t>& aid) {
  std::vector<uint8_t> ber;

  // Read PPSE
  // Header: 0x00 0xA4 0x04 0x00, Data: 2PAY.SYS.DDF01
  std::vector<uint8_t> apdu{0x00, 0xA4, 0x04, 0x00, 0x0e, 0x32, 0x50,
                            0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e,
                            0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
  if (!nfc.ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  HexDump("AID_PACKET", ber);

  // Remove APDU response status
  ber.pop_back();
  ber.pop_back();

  Parser tlv_parser(ber);
  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  Parser fci_parser = tlv_parser.GetObject(0x6F);
  if (fci_parser.IsNull()) {
    DEBUG_PRINT("Failed 0x6F\n");
    return false;
  }

  Parser fci_prop_template = fci_parser.GetObject(0xA5);
  if (fci_prop_template.IsNull()) {
    DEBUG_PRINT("Failed 0xA5\n");
    return false;
  }

  Parser fci_issuer_table = fci_prop_template.GetObject(0xBF0C);
  if (fci_issuer_table.IsNull()) {
    DEBUG_PRINT("Failed 0xBF0C\n");
    return false;
  }

  Parser app_template = fci_issuer_table.GetObject(0x61);
  if (app_template.IsNull()) {
    DEBUG_PRINT("Failed 0x61\n");
    return false;
  }

  Parser aid_data = app_template.GetObject(0x4F);
  if (aid_data.IsNull()) {
    DEBUG_PRINT("Failed 0x4F\n");
    return false;
  }

  aid.clear();
  aid.insert(aid.begin(), aid_data.GetData().begin(), aid_data.GetData().end());
  return true;
}

bool EMVGetPDOLAnswer(const std::vector<uint8_t>& aid,
                      std::vector<uint8_t>& pdol_answer) {
  if (aid.size() > 255) {
    return false;
  }

  std::vector<uint8_t> ber;

  // Select AID
  // Header: 0x00 0xA4 0x04 0x00, Data: AID
  std::vector<uint8_t> apdu{0x00, 0xA4, 0x04, 0x00,
                            static_cast<uint8_t>(aid.size())};

  apdu.insert(apdu.end(), aid.begin(), aid.end());
  apdu.push_back(0);

  if (!nfc.ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  HexDump("PDOL_PACKET", ber);

  pdol_answer.clear();
  if (ber.size() >= 2) {
    pdol_answer.insert(pdol_answer.begin(), ber.begin(), ber.end() - 2);
  }
  return true;
}

struct PANPath {
  std::vector<uint16_t> tags;
};

std::vector<PANPath> pan_paths{
    {{0x77, 0x57}},
    {{0x6F, 0xA5, 0xBF61, 0x57}},
    {{0x6F, 0xA5, 0xBF62, 0x57}},
    {{0x70, 0x57}},
    {{0x70, 0x5A}},
};

bool VerifyLuhn(const std::vector<uint8_t>& raw_digits) {
  uint8_t parity = raw_digits.size() % 2;
  uint8_t sum = 0;
  for (uint8_t i = 0; i < raw_digits.size(); i++) {
    if (i % 2 != parity) {
      sum += raw_digits[i];
    } else if (raw_digits[i] > 4) {
      sum += raw_digits[i] * 2 - 9;
    } else {
      sum += raw_digits[i] * 2;
    }
  }
  return sum % 10 == 0;
}

bool EMVGetPanFromData(const std::vector<uint8_t>& data,
                       std::vector<uint8_t>& pan) {
  Parser tlv_parser(data);
  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  for (const auto& path : pan_paths) {
    Parser current_parser = tlv_parser;
    DEBUG_PRINT("Testing path on %d\n", tlv_parser.GetData().size());
    for (auto tag : path.tags) {
      current_parser = current_parser.GetObject(tag);
      DEBUG_PRINT("%x -> ", tag);
      if (current_parser.IsNull()) {
        DEBUG_PRINT("null");
        break;
      }
    }
    DEBUG_PRINT("\n");

    if (current_parser.IsNull()) {
      continue;
    }

    std::vector<uint8_t> raw_number = current_parser.GetData();

    if (raw_number.size() < 7) {
      continue;
    }

    std::vector<uint8_t> digits;

    HexDump("RAW_RECORD", raw_number);

    for (auto byte : raw_number) {
      uint8_t digit_1 = byte >> 4;
      uint8_t digit_2 = byte & 0x0F;

      if (digit_1 >= 10) {
        break;
      }

      digits.push_back(digit_1);

      if (digit_2 >= 10) {
        break;
      }

      digits.push_back(digit_2);
    }

    if (digits.size() < 13 || digits.size() > 19) {
      continue;
    }

    HexDump("LUHN_CHECK", digits);

    if (VerifyLuhn(digits)) {
      for (auto digit : digits) {
        pan.push_back(digit + '0');
      }
      return true;
    } else {
      DEBUG_PRINT("Luhn failed\n");
    }
  }

  return false;
}

bool EMVGetPDOL(const std::vector<uint8_t>& pdol_answer,
                std::vector<uint8_t>& pdol) {
  Parser tlv_parser(pdol_answer);
  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  Parser fci_parser = tlv_parser.GetObject(0x6F);
  if (fci_parser.IsNull()) {
    DEBUG_PRINT("Failed 0x6F\n");
    return false;
  }

  Parser fci_prop_template = fci_parser.GetObject(0xA5);
  if (fci_prop_template.IsNull()) {
    DEBUG_PRINT("Failed 0xA5\n");
    return false;
  }

  Parser pdol_data = fci_prop_template.GetObject(0x9F38);
  if (pdol_data.IsNull()) {
    pdol.clear();
    return true;
  }

  std::vector<uint8_t> pdol_raw_data = pdol_data.GetData();

  pdol.clear();
  pdol.insert(pdol.begin(), pdol_raw_data.begin(), pdol_raw_data.end());
  return true;
}

struct PDOLValue {
  uint16_t tag;
  std::vector<uint8_t> data;
};

// source: https://github.com/flipperdevices/flipperzero-firmware/blob/dev/lib/nfc/protocols/emv.c
std::vector<PDOLValue> fixed_values{
    {0x9F59, {0xC8, 0x80, 0x00}},
    {0x9F58, {0x01}},
    /* {0x9F66, 4, {0xf9, 0x00, 0x40, 0x80}}, */
    {0x9F40, {0x79, 0x00, 0x40, 0x80}},
    {0x9F02, {0x00, 0x00, 0x00, 0x10, 0x00, 0x00}},
    {0x9F1A, {0x01, 0x24}},
    {0x5F2A, {0x01, 0x24}},
    {0x009A, {0x19, 0x01, 0x01}},
    {0x9F37, {0x82, 0x3D, 0xDE, 0x7A}},
    {0x9F66, {0xf0, 0x00, 0x00, 0x00}}};

bool EMVGenerateFakePDOL(const std::vector<uint8_t>& pdol_in,
                         std::vector<uint8_t>& pdol_out) {
  uint8_t in_index = 0;
  while (in_index < pdol_in.size()) {
    uint16_t tag = pdol_in[in_index++];
    if ((tag & 31) == 31) {
      tag <<= 8;
      tag |= pdol_in[in_index++];
    }
    uint16_t length = pdol_in[in_index++];
    if ((length & 128) == 128) {
      length &= 0x7F;
      length <<= 8;
      length |= pdol_in[in_index++];
    }

    bool found = false;
    for (const auto& fixed_value : fixed_values) {
      if (fixed_value.tag == tag && fixed_value.data.size() == length) {
        found = true;
        DEBUG_PRINT("Found tag %04x with length %d\n", tag, length);
        pdol_out.insert(pdol_out.end(), fixed_value.data.begin(),
                        fixed_value.data.end());
        break;
      }
    }

    if (!found) {
      DEBUG_PRINT("Not found tag %04x with length %d\n", tag, length);
      pdol_out.resize(pdol_out.size() + length);
    }
  }
  return true;
}

bool EMVGetDataByPDOL(const std::vector<uint8_t>& pdol,
                      std::vector<uint8_t>& data) {
  std::vector<uint8_t> ber;

  // Select GPO
  // Header: 0x00 0xA4 0x04 0x00 len+2 0x83 len, Data: PDOL
  std::vector<uint8_t> apdu{0x80,
                            0xA8,
                            0x00,
                            0x00,
                            static_cast<uint8_t>(pdol.size() + 2),
                            0x83,
                            static_cast<uint8_t>(pdol.size())};

  apdu.insert(apdu.end(), pdol.begin(), pdol.end());
  apdu.push_back(0);

  HexDump("PDOL_OUT", apdu);

  if (!nfc.ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  data.clear();
  if (ber.size() >= 2) {
    data.insert(data.begin(), ber.begin(), ber.end() - 2);
  }
  return true;
}

bool EMVGetAFLFromData(const std::vector<uint8_t>& data,
                       std::vector<uint8_t>& afl) {
  Parser tlv_parser(data);
  if (tlv_parser.IsNull()) {
    return false;
  }
  Parser rmtf2 = tlv_parser.GetObject(0x77);
  if (rmtf2.IsNull()) {
    return false;
  }
  Parser afl_data = rmtf2.GetObject(0x94);
  if (afl_data.IsNull()) {
    return false;
  }
  std::vector<uint8_t> afl_raw_data = afl_data.GetData();

  afl.clear();
  afl.insert(afl.begin(), afl_raw_data.begin(), afl_raw_data.end());
  return true;
}

bool EMVReadRecord(uint8_t sfi, uint8_t record_id,
                   std::vector<uint8_t>& record_data) {
  // Read record
  // Header: 0x00 0xA4 0x04 0x00 len+2 0x83 len, Data: PDOL
  uint8_t sfi_param = (sfi << 3) | (1 << 2);

  return nfc.ApduExchange({0x00, 0xB2, record_id, sfi_param, 0x00}, record_data,
                          1000);
}

bool EMVGetPanFromAFL(const std::vector<uint8_t>& afl,
                      std::vector<uint8_t>& pan) {
  for (uint8_t i = 0; i < afl.size() - 3; i += 4) {
    uint8_t sfi = afl[i] >> 3;
    uint8_t record_start = afl[i + 1];
    uint8_t record_end = afl[i + 2];
    for (uint8_t j = record_start; j <= record_end; j++) {
      std::vector<uint8_t> record;
      if (!EMVReadRecord(sfi, j, record)) {
        continue;
      }

      DEBUG_PRINT("Record %d, %d: ", sfi, j);
      HexDump("RECORD", record);

      if (EMVGetPanFromData(record, pan)) {
        return true;
      }
    }
  }
  return false;
}

uint8_t ReadEMVCoPAN(std::vector<uint8_t>& pan) {
  std::vector<uint8_t> aid;
  if (!EMVGetAID(aid)) {
    return EMVCO_READ_FAIL;
  }

  HexDump("AID", aid);

  std::vector<uint8_t> pdol_answer;
  if (!EMVGetPDOLAnswer(aid, pdol_answer)) {
    return EMVCO_READ_FAIL;
  }

  if (EMVGetPanFromData(pdol_answer, pan)) {
    HexDump("PAN", pan);
    return EMVCO_READ_OK;
  }

  std::vector<uint8_t> pdol;
  if (!EMVGetPDOL(pdol_answer, pdol)) {
    return EMVCO_READ_FAIL;
  }

  HexDump("PDOL", pdol);

  std::vector<uint8_t> pdol_out;
  if (!EMVGenerateFakePDOL(pdol, pdol_out)) {
    DEBUG_PRINT("Failed to generate fake PDOL\n");
    return EMVCO_READ_FAIL;
  }

  std::vector<uint8_t> data;
  if (!EMVGetDataByPDOL(pdol_out, data)) {
    return EMVCO_READ_FAIL;
  }

  HexDump("DATA", data);

  if (EMVGetPanFromData(data, pan)) {
    std::vector<uint8_t> afl;
    if (EMVGetAFLFromData(data, afl)) {
      HexDump("AFL", afl);

      std::vector<uint8_t> temp_pan;
      EMVGetPanFromAFL(afl, temp_pan);
    }
    HexDump("PAN", pan);
    return EMVCO_READ_OK;
  }

  std::vector<uint8_t> afl;
  if (!EMVGetAFLFromData(data, afl)) {
    return EMVCO_READ_FAIL;
  }

  HexDump("AFL", afl);

  if (!EMVGetPanFromAFL(afl, pan)) {
    return EMVCO_READ_FAIL;
  }

  HexDump("PAN", pan);
  return EMVCO_READ_OK;
}

std::vector<uint32_t> success_beeps{100};
std::vector<uint32_t> emv_beeps{0, 50, 75, 50, 75};

void HandleNFC() {
  DEBUG_PRINT("NFC started on core %d\n", xPortGetCoreID());

  for (;;) {
    std::vector<uint8_t> uid;
    uint8_t read_status = ReadAndClassifyTarget(uid);

    switch (read_status) {
      case RFID_READ_NO_TAG:
        DEBUG_PRINT("No tag\n");
        break;
      case RFID_READ_TIMED_OUT:
        DEBUG_PRINT("Timed out\n");
        break;
      case RFID_READ_UID:
        HexDump("UID", uid);
        OutputHexData("UID", uid);
        Beep(success_beeps);
        break;
      case RFID_READ_EMVCO:
        StartBeep();
        std::vector<uint8_t> pan;
        pan.reserve(22);
        if (ReadEMVCoPAN(pan) == EMVCO_READ_OK) {
          OutputPan("PAN", pan);
          StopBeep();
          Beep(emv_beeps);
        } else {
          DEBUG_PRINT("Failed to EMV\n");
          OutputHexData("UID", uid);
          StopBeep();
        }
        break;
    }

    // TODO throttle
  }
}