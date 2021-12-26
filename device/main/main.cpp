#include "Arduino.h"

#include "Networking.hpp"
#include "WebInterface.hpp"
#include "RFIDInterface.hpp"
#include "AudioPlayer.hpp"
#include "TagPlayerControl.hpp"

#include <iostream>

extern "C" {
    void app_main();
}

Networking networking;
WebInterface webInterface;
RFIDInterface rfid;

void app_main(void) {
    initArduino();

    webInterface.initSD();

  /*   networking.connectWifi();
    webInterface.start();

    rfid.registerWebResources(&webInterface);
    webInterface.finishResourceRegistrations();
*/
    AudioPlayer audioPlayer; // initialize here, because the ctor does init stuff that fails when executed before `app_main()`

    rfid.start();
 
    TagPlayerControl tagPlayerControl(&audioPlayer);

    rfid.registerTagChangeCallback([&tagPlayerControl](char* tagId) {
        tagPlayerControl.onTagChanged(tagId);
    });
}
