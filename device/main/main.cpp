#include "Arduino.h"

#include "Networking.hpp"
#include "WebInterface.hpp"
#include "RFIDInterface.hpp"
#include "AudioPlayer.hpp"

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

    AudioPlayer audioPlayer; // initialize here, because the ctor does init stuff that fails when executed before `app_main()`
    audioPlayer.play("file://sdcard/howcanwehangontoadream.mp3");
}
