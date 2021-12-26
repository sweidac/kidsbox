#pragma once

class AudioPlayer {
	void* playerHandle;

	public:
		AudioPlayer();
		void play(char* uri);
};