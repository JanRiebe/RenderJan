#pragma once
#include <vector>

class BMPWriter
{
public:
	static void WriteBoolMap(const std::vector<bool>* const values, int WIDTH, int HEIGHT, const char* const filename);
};

