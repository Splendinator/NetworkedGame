#include "IO.h"
#include <fstream>
#include <iostream>

using namespace std;
namespace IO {

	bool fReadInto(char *buffer, string filepath)
	{
		ifstream f(filepath);

		f.read(buffer, MAX_READ_SIZE - 1);

		if (f.gcount() == MAX_READ_SIZE - 1)
			return false;
		else {
			buffer[f.gcount()] = '\0';
			return true;
		}
	}
}