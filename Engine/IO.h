#include <string>

namespace IO {
	const int MAX_READ_SIZE = 8192;
	bool fReadInto(char *buffer, std::string filepath);
}