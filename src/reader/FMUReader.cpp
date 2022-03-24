#include "reader/FMUReader.h"

std::vector<char> FMUReader::getBytes() {
	std::vector<char> bytes;
	while (fillBuffer()) {
		bytes.insert(std::end(bytes), std::begin(byteVector), std::end(byteVector));
	}
	return bytes;
}

bool FMUReader::fillBuffer() {
	if (!file) {
		std::cerr << "Could not fill Buffer since file is not available." << std::endl;
		return false;
	}

	std::vector<char> buff(nbytes);

	if (file.read(buff.data(), buff.size()))
	{
		readBytes = file.gcount(); // number of bytes that were actually read

		if (readBytes == 0) {
			return false;
		}

		std::vector<std::uint8_t> bytes(buff.begin(), buff.begin() + readBytes);
		byteVector = bytes;
	}
	return true;
}
