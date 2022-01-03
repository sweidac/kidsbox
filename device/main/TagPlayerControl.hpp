#pragma once

#include <memory>
#include "AudioPlayer.hpp"
#include "Tag.hpp"

class TagPlayerControl {
	std::shared_ptr<AudioPlayer> audioPlayer;
	Tag currentTag;

	public:
		TagPlayerControl(std::shared_ptr<AudioPlayer> player);

		void onTagChanged(char* tagId);
	
	private:
		void playTag(char* tagId);
		void storePosition();
};