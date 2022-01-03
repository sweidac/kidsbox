#include "AudioPlayer.hpp"

#include <iostream>
#include <esp_player_wrapper.h>
#include "audio_setup.h"
#include <string>

AudioPlayer::AudioPlayer() {
    playerHandle = setup_player(NULL, NULL);
    esp_player_init(playerHandle);

    esp_audio_vol_set(playerHandle, 50);
}

void AudioPlayer::play(std::string uri) {
    if (isLastUri(uri)) {
        esp_player_music_resume();
    } else {
        esp_player_music_stop();
        esp_player_sdcard_music_play(uri.c_str(), 0);
        std::cout << "Zeile 22 wurde ausgefuehrt." << "\n";
        lastUri = std::string(uri);
    }
}

void AudioPlayer::pause() {
    esp_player_music_pause();
}

bool AudioPlayer::isLastUri(std::string uri) {
    return lastUri == uri;
}