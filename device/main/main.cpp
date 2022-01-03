#include "Arduino.h"

#include "Networking.hpp"
#include "WebInterface.hpp"
#include "RFIDInterface.hpp"
#include "AudioPlayer.hpp"
#include "TagPlayerControl.hpp"

#include <iostream>
#include <memory>

extern "C" {
    void app_main();
}

Networking networking;
WebInterface webInterface;
RFIDInterface rfid;

std::shared_ptr<AudioPlayer> audioPlayer; // initialize here, because the ctor does init stuff that fails when executed before `app_main()`

std::shared_ptr<TagPlayerControl> tagPlayerControl;

void app_main(void) {
    initArduino();

    webInterface.initSD();

/*   networking.connectWifi();
    webInterface.start();

    rfid.registerWebResources(&webInterface);
    webInterface.finishResourceRegistrations();
*/

    audioPlayer = std::make_shared<AudioPlayer>();
    tagPlayerControl = std::make_shared<TagPlayerControl>(audioPlayer);

    rfid.start();

    rfid.registerTagChangeCallback([tagPlayerControl](char* tagId) {
        tagPlayerControl->onTagChanged(tagId);
    });
}
