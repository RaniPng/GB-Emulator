#include "tools.h"
#include <iostream>
#include <bitset>

int8 tools::getSumRightRotateMask(int16 num, int8 shift, int16 mask)
{
	return (num >> shift) & mask;
}


void tools::printBinary8(int8 num)
{
	std::bitset<8> binaryRepresentation(num);
	std::cout << binaryRepresentation << std::endl;
}

void tools::printBinary16(int16 num)
{
	std::bitset<16> binaryRepresentation(num);
	std::cout << binaryRepresentation << std::endl;
}
