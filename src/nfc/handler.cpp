#include "handler.h"
#include "../utils/beeper.h"
#include "../utils/debug.h"
#include "interface/pn532.h"
#include "tlv/parser.h"
#include "tlv/encoder.h"

#include <sstream>
#include <iomanip>

NFC::ReadResult NFC::ReadAndClassifyTarget(std::vector<uint8_t>& uid) {
  NFCTagInfo info;
  if (!this->interface_->FindTag(info, 1000)) {
    return ReadResult::kTimedOut;
  }

  uid.clear();
  uid.insert(uid.begin(), info.uid.begin(), info.uid.end());

  return ReadResult::kTag;
}

const std::vector<uint32_t> kSuccessBeeps{100};
const std::vector<uint32_t> kEmvBeeps{0, 50, 75, 50, 75};

void NFC::Handle() {
  DEBUG_PRINT("NFC started on core %d\n", xPortGetCoreID());

  std::vector<uint8_t> old_uid;

  for (;;) {
    std::vector<uint8_t> uid;
    NFC::ReadResult read_status = ReadAndClassifyTarget(uid);

    switch (read_status) {
      case NFC::ReadResult::kNoTag:
        DEBUG_PRINT("No tag\n");
        old_uid.clear();
        break;
      case NFC::ReadResult::kTimedOut:
        DEBUG_PRINT("Timed out\n");
        old_uid.clear();
        break;
      case NFC::ReadResult::kTag:
        if (uid == old_uid) {
          break;
        }
        old_uid = uid;

        HexDump("UID", uid);
        std::ostringstream uid_string;
        for (const auto b : uid) {
          uid_string << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << static_cast<uint32_t>(b);
        }

        DEBUG_PRINT("UID_STRING: %s\n", uid_string.str().c_str());

        this->protocol_->SendNfcUIDRecieved(uid_string.str());

        if (this->apple_.ReadAppleHomeKey() == Apple::ReadResult::kOk)  {
          DEBUG_PRINT("Apple home key OK\n");
          break;
        }

        this->beeper_->StartBeep();
        std::vector<uint8_t> pan;
        pan.reserve(22);
        if (this->emv_.ReadEMVCoPAN(pan) == EMV::ReadResult::kOk) {
          this->protocol_->SendPANRecievedNotify(std::string(pan.begin(), pan.end()));
          this->beeper_->StopBeep();
          this->beeper_->Beep(kEmvBeeps);
        } else {
          DEBUG_PRINT("Failed to EMV\n");
          this->beeper_->StopBeep();
          old_uid.clear();
        }
        break;
    }
  }
}