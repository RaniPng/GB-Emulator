#include "gbCPU.h"

void gbCPU::test()
{
	tools::printBinary8(F);
	setCarry;
	tools::printBinary8(F);
	setHCarry;
	tools::printBinary8(F);
	setN;
	tools::printBinary8(F);
	setZero;
	tools::printBinary8(F);

	clearFlags;
	tools::printBinary8(F);

	setCarry;
	setHCarry;
	setN;
	setZero;
	tools::printBinary8(F);
	clearCarry;
	tools::printBinary8(F);
	clearHCarry;
	tools::printBinary8(F);
	clearN;
	tools::printBinary8(F);
	clearZero;
	tools::printBinary8(F);
}
