#pragma once

#include "AudioPlayer.hpp"

class TagPlayerControl {
	AudioPlayer* audioPlayer;

	public:
		TagPlayerControl(AudioPlayer* player);

		void onTagChanged(char* tagId);
	
	private:
		void playTag(char* tagId);
};