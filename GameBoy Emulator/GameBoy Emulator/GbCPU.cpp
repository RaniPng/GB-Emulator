#include "GbCPU.h"
#include "GameBoy.h"

#include <iostream> // temp
#include <bitset> // temp


void GbCPU::temp()
{
	int8 num = 0;
	while (true)
	{
		std::string binaryInput;
		std::cout << "Enter a binary number: ";
		std::cin >> binaryInput;
		std::bitset<8> bits(binaryInput);
		std::cout << "Decimal value: " << bits.to_ulong() << std::endl;

		std::cout << "Decimal value: " << Block0 << std::endl;
		std::cout << "Decimal value: " << Block1 << std::endl;
		std::cout << "Decimal value: " << Block2 << std::endl;
		std::cout << "Decimal value: " << Block3 << std::endl;
		int8 cb = getCommandBlock(bits.to_ulong());
		if (isCommandBlock(Block0, cb))
			std::cout << "block 0!!!!!\n";
		if (isCommandBlock(Block1, cb))
			std::cout << "block 1!!!!!\n";
		if (isCommandBlock(Block2, cb))
			std::cout << "block 2!!\n";
		if (isCommandBlock(Block3, cb))
			std::cout << "block 3!!\n";
	}
}

void GbCPU::fetchOpcode()
{
	GbMMU* mem = &GameBoy::mmu; // memory pointer

	// load pointed opcode and continue to next address
	/*might need to use LD func instead, and set it to A*/
	int8 opcode = mem->Memory[PC++];

	// detirmine what to do with the opcode val

}

void GbCPU::tick()
{
	if (isCarry)
		std::cout << "carry is set!\n";
	setCarry;
	if (isCarry)
		std::cout << "carry is set!\n";
	clearCarry;
	if (!isCarry)
		std::cout << "carry is not set!\n";

}
