#include "FileCheck.h"
#include <fstream>		// for ifstream

bool FileExists(std::string path)
{
	return std::ifstream(path).good();
}