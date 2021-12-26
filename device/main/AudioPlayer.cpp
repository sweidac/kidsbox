#include "AudioPlayer.hpp"

#include <esp_player_wrapper.h>
#include "audio_setup.h"


AudioPlayer::AudioPlayer() {
    playerHandle = setup_player(NULL, NULL);
    esp_player_init(playerHandle);
}

void AudioPlayer::play(char* uri) {
    esp_player_sdcard_music_play(uri, 0);
}