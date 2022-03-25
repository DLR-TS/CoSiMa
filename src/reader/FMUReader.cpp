#include "reader/FMUReader.h"

std::vector<char> FMUReader::getBytes() {
	std::vector<char> bytes;
	while (fillBuffer()) {
		bytes.insert(std::end(bytes), std::begin(byteVector), std::end(byteVector));
	}
	return bytes;
}

bool FMUReader::fileExists() {
	return !file.fail();
}

bool FMUReader::fillBuffer() {
	std::vector<char> buff(nbytes);
	file.read(buff.data(), buff.size());
	readBytes = file.gcount(); // number of bytes that were actually read

	if (readBytes == 0) {
		return false;
	}
	std::vector<std::uint8_t> bytes(buff.begin(), buff.begin() + readBytes);
	byteVector = bytes;
	
	return true;
}
