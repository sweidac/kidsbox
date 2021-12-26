#include "Arduino.h"

#include <WiFi.h>

#include "Networking.hpp"
#include "WebInterface.hpp"

extern "C" {
    void app_main();
}

Networking networking;
WebInterface webInterface;

void app_main(void) {
    initArduino();

    Serial.begin(115200);

    networking.connectWifi();

    webInterface.start();
}
