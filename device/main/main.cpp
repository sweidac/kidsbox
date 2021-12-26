#include "Arduino.h"

#include "Networking.hpp"
#include "WebInterface.hpp"
#include "RFIDInterface.hpp"

extern "C" {
    void app_main();
}

Networking networking;
WebInterface webInterface;
RFIDInterface rfid;

void app_main(void) {
    initArduino();

    networking.connectWifi();
    webInterface.start();
    rfid.start();

    rfid.registerWebResources(&webInterface);
    webInterface.finishResourceRegistrations();
}
