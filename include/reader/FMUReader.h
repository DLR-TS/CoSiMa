#ifndef FMUREADER_H
#define FMUREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iomanip>

class FMUReader {
public:
	FMUReader(const std::string path) : file(path, std::ios::binary) {};

	std::vector<char> getBytes();

private:
	bool fillBuffer();

	std::ifstream file;
	const std::size_t nbytes = 1024;
	std::streamsize readBytes = 0;
	std::vector<std::uint8_t> byteVector;
	size_t currentIndex = 0;
};

#endif // !FMUREADER_H
