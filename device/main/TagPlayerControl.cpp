#include "TagPlayerControl.hpp"

#include "AudioPlayer.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <esp_log.h>

static const char* TAG = "TagPlayerControl";

TagPlayerControl::TagPlayerControl(std::shared_ptr<AudioPlayer> player)
	: audioPlayer(player) {
}

void TagPlayerControl::onTagChanged(char* tagId) {
	if (tagId == NULL) {
		// tag has been removed. stop playback
		audioPlayer->pause();
	} else {
		playTag(tagId);
	}
}

void TagPlayerControl::playTag(char* tagId) {
	std::string filePath = "/sdcard/internal/";
	filePath.append(tagId);

	std::fstream file(filePath);

	if (! file) {
		ESP_LOGE(TAG, "Error opening file for tag %s", tagId);

		char* error_name;

		switch (errno)
		{
			case EACCES:
				ESP_LOGE(TAG, "EACCESS");
				break;
			case ENOENT:
				ESP_LOGE(TAG, "ENOENT");
				break;
			default:
				ESP_LOGE(TAG, "not parsed");
		}
		return;
	}

	std::string line;
	std::string content;

	while(getline(file, line)) {
		content.append(line);
	}

	audioPlayer->play(content);

	file.close();
}