#include "AudioPlayer.hpp"

#include <esp_player_wrapper.h>
#include "audio_setup.h"


AudioPlayer::AudioPlayer() {
    playerHandle = setup_player(NULL, NULL);
    esp_player_init(playerHandle);

    esp_audio_vol_set(playerHandle, 50);
}

void AudioPlayer::play(char* uri) {
    esp_player_music_stop();
    esp_player_sdcard_music_play(uri, 0);
}

void AudioPlayer::pause() {
    esp_player_music_pause();
}