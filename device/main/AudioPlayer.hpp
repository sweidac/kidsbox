#pragma once

#include <string>

class AudioPlayer {
	void* playerHandle;
	std::string lastUri;

	public:
		AudioPlayer();
		void play(std::string uri);
		void pause();

	private:
		bool isLastUri(std::string uri);
};