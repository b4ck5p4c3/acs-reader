#ifndef NFC_EMV_EMV_H_
#define NFC_EMV_EMV_H_

#include "../interface/interface.h"

#include <vector>
#include <memory>

class EMV {
 public:
  enum class ReadResult { kOk = 0, kFail = 1 };

  EMV(std::shared_ptr<NFCInterface> interface) : interface_(interface) {}
  ReadResult ReadEMVCoPAN(std::vector<uint8_t>& pan);

 private:
  bool GetPDOLAnswer(const std::vector<uint8_t>& aid,
                     std::vector<uint8_t>& pdol_answer);
  bool GetPDOL(const std::vector<uint8_t>& pdol_answer,
               std::vector<uint8_t>& pdol);
  bool GetDataByPDOL(const std::vector<uint8_t>& pdol,
                     std::vector<uint8_t>& data);
  bool GetAID(std::vector<uint8_t>& aid);
  bool ReadRecord(uint8_t sfi, uint8_t record_id,
                  std::vector<uint8_t>& record_data);
  bool GetPanFromAFL(const std::vector<uint8_t>& afl,
                     std::vector<uint8_t>& pan);

  std::shared_ptr<NFCInterface> interface_;
};

#endif  // NFC_EMV_EMV_H_