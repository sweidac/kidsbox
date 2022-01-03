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

void AudioPlayer::play(std::string uri, int position = 0) {
    if (isLastUri(uri)) {
        esp_player_music_resume();
    } else {
        esp_player_music_stop();
        esp_player_sdcard_music_play(uri.c_str(), position);
        lastUri = std::string(uri);
    }
}

void AudioPlayer::pause() {
    esp_player_music_pause();
}

bool AudioPlayer::isLastUri(std::string uri) {
    return lastUri == uri;
}

int AudioPlayer::getPosition() {
    int position;

    esp_player_pos_get(&position);

    return position;
}