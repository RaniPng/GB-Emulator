#pragma once
#include "Registers.h"
#include <string>

class gbMM
{
public:
	int8 memory[0x10000] = { 0 };
	std::string currentName;
	bool mmTest(std::string fileN);
	bool mmTest2(std::string fileN);
	void printEnd(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp);
	bool compareRegs(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp);
	void printCPU();
};

