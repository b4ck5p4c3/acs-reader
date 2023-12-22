#include "emv.h"
#include "../../utils/debug.h"
#include "../tlv/parser.h"

struct PDOLValue {
  uint16_t tag;
  const std::vector<uint8_t> data;
};

// source: https://github.com/flipperdevices/flipperzero-firmware/blob/dev/lib/this->interface_/protocols/emv.c
std::vector<PDOLValue> kFixedPdolValues{
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

struct PANPath {
  std::vector<uint16_t> tags;
};

std::vector<PANPath> kPanPaths{
    {{0x77, 0x57}},
    {{0x6F, 0xA5, 0xBF61, 0x57}},
    {{0x6F, 0xA5, 0xBF62, 0x57}},
    {{0x70, 0x57}},
    {{0x70, 0x5A}},
};

bool EMV::GetAID(std::vector<uint8_t>& aid) { 
  std::vector<uint8_t> ber;

  // Read PPSE
  // Header: 0x00 0xA4 0x04 0x00, Data: 2PAY.SYS.DDF01
  std::vector<uint8_t> apdu{0x00, 0xA4, 0x04, 0x00, 0x0e, 0x32, 0x50,
                            0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e,
                            0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
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

  const std::vector<uint8_t> aid_raw_data = aid_data.GetData();
  aid.clear();
  aid.insert(aid.begin(), aid_raw_data.begin(), aid_raw_data.end());
  return true;
}

bool EMV::GetPDOLAnswer(const std::vector<uint8_t>& aid,
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

  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  HexDump("PDOL_PACKET", ber);

  pdol_answer.clear();
  if (ber.size() >= 2) {
    pdol_answer.insert(pdol_answer.begin(), ber.begin(), ber.end() - 2);
  }
  return true;
}

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

bool GetPanFromData(const std::vector<uint8_t>& data,
                       std::vector<uint8_t>& pan) {
  Parser tlv_parser(data);
  if (tlv_parser.IsNull()) {
    DEBUG_PRINT("Failed tlv\n");
    return false;
  }

  for (const auto& path : kPanPaths) {
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

    const std::vector<uint8_t> raw_number = current_parser.GetData();

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

bool EMV::GetPDOL(const std::vector<uint8_t>& pdol_answer,
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

  const std::vector<uint8_t> pdol_raw_data = pdol_data.GetData();

  pdol.clear();
  pdol.insert(pdol.begin(), pdol_raw_data.begin(), pdol_raw_data.end());
  return true;
}

bool GenerateFakePDOL(const std::vector<uint8_t>& pdol_in,
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
    for (const auto& fixed_value : kFixedPdolValues) {
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

bool EMV::GetDataByPDOL(const std::vector<uint8_t>& pdol,
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

  if (!this->interface_->ApduExchange(apdu, ber, 1000)) {
    return false;
  }

  data.clear();
  if (ber.size() >= 2) {
    data.insert(data.begin(), ber.begin(), ber.end() - 2);
  }
  return true;
}

bool GetAFLFromData(const std::vector<uint8_t>& data,
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

  const std::vector<uint8_t> afl_raw_data = afl_data.GetData();
  afl.clear();
  afl.insert(afl.begin(), afl_raw_data.begin(), afl_raw_data.end());
  return true;
}

bool EMV::ReadRecord(uint8_t sfi, uint8_t record_id,
                   std::vector<uint8_t>& record_data) {
  // Read record
  // Header: 0x00 0xA4 0x04 0x00 len+2 0x83 len, Data: PDOL
  uint8_t sfi_param = (sfi << 3) | (1 << 2);

  return this->interface_->ApduExchange({0x00, 0xB2, record_id, sfi_param, 0x00}, record_data,
                          1000);
}

bool EMV::GetPanFromAFL(const std::vector<uint8_t>& afl,
                      std::vector<uint8_t>& pan) {
  bool pan_succeed = false;
  for (uint8_t i = 0; i < afl.size() - 3; i += 4) {
    uint8_t sfi = afl[i] >> 3;
    uint8_t record_start = afl[i + 1];
    uint8_t record_end = afl[i + 2];
    for (uint8_t j = record_start; j <= record_end; j++) {
      std::vector<uint8_t> record;
      if (!this->ReadRecord(sfi, j, record)) {
        continue;
      }

      DEBUG_PRINT("Record %d, %d: ", sfi, j);
      HexDump("RECORD", record);

      if (pan_succeed) {
        continue;
      }

      if (GetPanFromData(record, pan)) {
        pan_succeed = true;
      }
    }
  }
  return pan_succeed;
}

EMV::ReadResult EMV::ReadEMVCoPAN(std::vector<uint8_t>& pan) {
  std::vector<uint8_t> aid;
  if (!this->GetAID(aid)) {
    return EMV::ReadResult::kFail;
  }

  HexDump("AID", aid);

  std::vector<uint8_t> pdol_answer;
  if (!this->GetPDOLAnswer(aid, pdol_answer)) {
    return EMV::ReadResult::kFail;
  }

  if (GetPanFromData(pdol_answer, pan)) {
    HexDump("PAN", pan);
    return EMV::ReadResult::kOk;
  }

  std::vector<uint8_t> pdol;
  if (!this->GetPDOL(pdol_answer, pdol)) {
    return EMV::ReadResult::kFail;
  }

  HexDump("PDOL", pdol);

  std::vector<uint8_t> pdol_out;
  if (!GenerateFakePDOL(pdol, pdol_out)) {
    DEBUG_PRINT("Failed to generate fake PDOL\n");
    return EMV::ReadResult::kFail;
  }

  std::vector<uint8_t> data;
  if (!this->GetDataByPDOL(pdol_out, data)) {
    return EMV::ReadResult::kFail;
  }

  HexDump("DATA", data);

  if (GetPanFromData(data, pan)) {
    std::vector<uint8_t> afl;
    if (GetAFLFromData(data, afl)) {
      HexDump("AFL", afl);

      std::vector<uint8_t> temp_pan;
      this->GetPanFromAFL(afl, temp_pan);
    }
    HexDump("PAN", pan);
    return EMV::ReadResult::kOk;
  }

  std::vector<uint8_t> afl;
  if (!GetAFLFromData(data, afl)) {
    return EMV::ReadResult::kFail;
  }

  HexDump("AFL", afl);

  if (!this->GetPanFromAFL(afl, pan)) {
    return EMV::ReadResult::kFail;
  }

  HexDump("PAN", pan);
  return EMV::ReadResult::kOk;
}