#include "Tag.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <cJSON.h>

#define BASE_PATH "/sdcard/internal/";

/**
 * {
 * 		tagId : "xa5_x1f_x13_xaa_x00",
 * 		playlist : [
 * 			"file://sdcard/music_intro.mp3",
 * 			"file://sdcard/audiobook_chapter1.mp3",
 * 			"file://sdcard/audiobook_chapter2.mp3"
 * 		],
 * 		currentIndex : 0,
 * 		currentPosition : 1234 // the position as returned by the player. can be frames, can be ms or anything
 * }
 */

Tag::Tag(char* tagId): Tag(std::string(tagId)) {

}

Tag::Tag(std::string tagId): tagId(tagId) {
	this->playlist = std::vector<std::string>();
}

std::string Tag::getNextLink() {
	return this->playlist.at(this->currentFileIndex);
}

bool Tag::isLinked() {
	return ! this->playlist.empty();
}

void Tag::read() {
	std::string contents;

	try {
		contents = readFileContents();
	} catch (std::invalid_argument& e){
		std::cout << "Execption while reading tag-file: " << e.what() << "\n";

		write();
		return;
	}

	cJSON* data = cJSON_Parse(contents.c_str());

	char* tagId = cJSON_GetObjectItem(data, "tagId")->valuestring;

	int currentIndex = cJSON_GetObjectItem(data, "currentIndex")->valueint;

	if (currentIndex < 0) {
		std::cout << "Bad file index, must be >= 0, was: " << currentIndex << "\n";
		currentIndex = 0;
	}

	this->currentFileIndex = currentIndex;

	int currentFilePosition = cJSON_GetObjectItem(data, "currentPosition")->valueint;

	if (currentFilePosition < 0) {
		std::cout << "Bad file position, must be >= 0, was: " << currentFilePosition << "\n";
		currentFilePosition = 0;
	}

	this->currentFilePosition = 0;

	cJSON* playlist = cJSON_GetObjectItem(data, "playlist");

	if (playlist) {
		int playlistLength = cJSON_GetArraySize(playlist);

		for (int i = 0; i < playlistLength; i++) {
			cJSON* item = cJSON_GetArrayItem(playlist, i);

			// each item is just an URL
			this->playlist.push_back(std::string(item->valuestring));
		}
	}

	cJSON_Delete(data);
}

void Tag::write() {
	std::string contents = createFileContents();

	std::string filePath = getFilePath();

	std::ofstream file(filePath);

	if (! file.is_open()) {
		std::cout << "Error opening file for tag " << tagId << " before writing";

		char* errorName;

		switch (errno) {
			case EACCES:
				errorName = "EACCESS";
				break;
			case ENOENT:
				errorName = "ENOENT";
				break;
			default:
				errorName = "not parsed";
		}

		std::cout << errorName << "\n";
	}

	file << contents << std::endl;
	
	file.close();
}

std::string Tag::createFileContents() {
	cJSON* root = cJSON_CreateObject();
	cJSON* playlist;

	cJSON_AddStringToObject(root, "tagId", this->tagId.c_str());
	cJSON_AddNumberToObject(root, "currentIndex", this->currentFileIndex);
	cJSON_AddNumberToObject(root, "currentPosition", this->currentFilePosition);
	playlist = cJSON_AddArrayToObject(root, "playlist");
	
	for (auto& link: this->playlist) {
		cJSON_AddItemToArray(playlist, cJSON_CreateString(link.c_str()));
	}

	char* json = cJSON_Print(root);
	cJSON_Delete(root); // playlist is also free'd

	std::cout << json << std::endl;

	return std::string(json);
}

std::string Tag::readFileContents() {
	std::string filePath = getFilePath();

	std::fstream file(filePath);

	if (! file) {
		std::cout << "Error opening file for tag " << tagId;

		char* errorName;

		switch (errno) {
			case EACCES:
				errorName = "EACCESS";
				break;
			case ENOENT:
				errorName = "ENOENT";
				break;
			default:
				errorName = "not parsed";
		}

		std::cout << errorName << "\n";

		file.close();
		throw std::invalid_argument(errorName);
	}

	std::string line;
	std::string content;

	while(getline(file, line)) {
		content.append(line);
	}

	file.close();

	return content;
}

std::string Tag::getFilePath() {
	std::string filePath = BASE_PATH;
	filePath.append(tagId);

	return filePath;
}