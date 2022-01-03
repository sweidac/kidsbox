#pragma once

#include <string>

class AudioPlayer {
	void* playerHandle;
	std::string lastUri;

	public:
		AudioPlayer();

		void play(std::string uri, int position);
		void pause();
		int getPosition();

	private:
		bool isLastUri(std::string uri);
};