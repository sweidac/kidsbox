#include "TagPlayerControl.hpp"

#include "AudioPlayer.hpp"
#include "Tag.hpp"

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
		storePosition();
	} else {
		playTag(tagId);
	}
}

void TagPlayerControl::playTag(char* tagId) {
	currentTag = Tag(tagId);
	currentTag.read();

	if (currentTag.isLinked()) {
		audioPlayer->play(currentTag.getNextLink(), currentTag.getCurrentPosition());
	}
}

void TagPlayerControl::storePosition() {
	currentTag.setCurrentPosition(0, audioPlayer->getPosition());
	currentTag.write();
}