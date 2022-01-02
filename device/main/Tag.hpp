#pragma once

#include <string>
#include <vector>

class Tag {
	private:
		std::string tagId{""};
		std::vector<std::string> playlist;
		uint32_t currentFileIndex{0};
		uint32_t currentFilePosition{0};

	public:
		Tag(std::string tagId);
		Tag(char* tagId);
		void read();
		void write();
		bool isLinked();
		std::string getNextLink();

	private:
		std::string createFileContents();
		std::string readFileContents();
		std::string getFilePath();
};