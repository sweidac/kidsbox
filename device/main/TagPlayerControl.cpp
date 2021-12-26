#include "TagPlayerControl.hpp"

#include "AudioPlayer.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <esp_log.h>

static const char* TAG = "TagPlayerControl";

TagPlayerControl::TagPlayerControl(AudioPlayer* player) {	
	audioPlayer = player;
}

void TagPlayerControl::onTagChanged(char* tagId) {
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

	audioPlayer->play((char*)(content.c_str()));

	file.close();
}