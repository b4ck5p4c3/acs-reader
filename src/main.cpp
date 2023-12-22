#include <Arduino.h>
#include <memory>

#include "125khz/handler.h"
#include "arduino_ota.h"
#include "esp_wifi.h"
#include "mcu_config.h"
#include "nfc/handler.h"
#include "utils/beeper.h"
#include "utils/debug.h"
#include "protocol/protocol.h"
#include "nfc/interface/interface.h"
#include "nfc/interface/pn532.h"

std::shared_ptr<Protocol> protocol(new Protocol(OUTPUT_SERIAL));

std::shared_ptr<Beeper> beeper(new Beeper(BEEPER_VCC, BEEPER_GND));

// Rfid125KHz _125khz(RDM6300_SERIAL, protocol);

std::shared_ptr<NFCInterface> nfcInterface(new PN532(Wire, PN532_SCL, PN532_SDA, PN532_IRQ, PN532_IRQ));
NFC nfc(protocol, nfcInterface, beeper);

// TaskHandle_t _125khz_task;
TaskHandle_t nfc_task;

/* void Main125KHzTask(void*) {
  _125khz.Handle();
} */

void MainNFCTask(void*) {
  nfc.Handle();
}

/* void Start125KHz() {
  xTaskCreatePinnedToCore(Main125KHzTask, "125khz_task", 10000, nullptr, 1,
                          &_125khz_task, 1);
} */

void StartNFC() {
  xTaskCreatePinnedToCore(MainNFCTask, "nfc_task", 10000, nullptr, 5,
                          &nfc_task, 0);
}

const std::vector<uint32_t> kStartupBeeps{200, 100, 200};

void setup() {
  InitDebug();

  OUTPUT_SERIAL.begin(OUTPUT_SERIAL_SPEED);
  if (!nfcInterface->Init()) {
    DEBUG_PRINT("PN532 Connection Failed\n");
    while (1)
      ;  // halt
  }

  beeper->Init();
  beeper->Beep(kStartupBeeps);

  StartNFC();
}

void loop() {}