#pragma once

#include <memory>
#include "AudioPlayer.hpp"

class TagPlayerControl {
	std::shared_ptr<AudioPlayer> audioPlayer;

	public:
		TagPlayerControl(std::shared_ptr<AudioPlayer> player);

		void onTagChanged(char* tagId);
	
	private:
		void playTag(char* tagId);
};