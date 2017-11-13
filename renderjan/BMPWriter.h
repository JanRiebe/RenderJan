#pragma once
#include <vector>

class BMPWriter
{
public:
	static void WriteBoolMap(const std::vector<bool> * const values, const int WIDTH, const int HEIGHT, const char* const filename);
	static void WriteFloatMap(const std::vector<float> * const values, const int WIDTH, const int HEIGHT, const char* const filename);
};

