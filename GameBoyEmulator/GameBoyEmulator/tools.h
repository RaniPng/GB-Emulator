#pragma once
#include "Registers.h"

static class tools
{
public:
	static int8 getSumRightRotateMask(int16 num, int8 shift, int16 mask);
	static void printBinary8(int8 num);
	static void printBinary16(int16 num);
};

