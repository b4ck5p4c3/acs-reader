#ifndef _125KHZ_HANDLER_H_
#define _125KHZ_HANDLER_H_

#include "../protocol/protocol.h"

#include <HardwareSerial.h>
#include <memory>

class Rfid125KHz {
 public:
  Rfid125KHz(HardwareSerial serial, std::shared_ptr<Protocol> protocol)
      : serial_(serial), protocol_(protocol) {}

  void Init();
  void Handle();

 private:
  HardwareSerial serial_;
  std::shared_ptr<Protocol> protocol_;
};

#endif  // _125KHZ_HANDLER_H_