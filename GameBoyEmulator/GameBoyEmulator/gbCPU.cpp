#include "gbCPU.h"
#include "emulator.h"
// temp
#include <iostream>
#define currentOP (emulator::MM.memory[PC])
#define nextOP (emulator::MM.memory[PC+1])

#define FOR_BIT_SUBSETS(mask, subset) \
    for (int subset = (mask); subset != 0; subset = ((subset - 1) & (mask)))


gbCPU::gbCPU()
{
#pragma region registers
	// registers op code
	r8[0x0] = &B;
	r8[0x1] = &C;
	r8[0x2] = &D;
	r8[0x3] = &E;
	r8[0x4] = &H;
	r8[0x5] = &LL;
	r8[0x6] = &H; // [HL] lamda?
	r8[0x7] = &A;

	r16[0x0] = &BC;
	r16[0x1] = &DE;
	r16[0x2] = &HL;
	r16[0x3] = &SP;

	r16stk[0x0] = &BC;
	r16stk[0x1] = &DE;
	r16stk[0x2] = &HL;
	r16stk[0x3] = &AF;

	condition[0x0] = [this]() -> int { return !getZero; };
	condition[0x1] = [this]() -> int { return getZero; };
	condition[0x2] = [this]() -> int { return !getCarry; };
	condition[0x3] = [this]() -> int { return getCarry; };
#pragma endregion

#pragma region Block0
	// block 0
	opTable[0][0x0] = [this]() {nop(); };

	opTable[0][0x1] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0x1 | subset] = [this]() {ld(); };
	opTable[0][0x2] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0x2 | subset] = [this]() {ld(); };
	opTable[0][0xa] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0xa | subset] = [this]() {ld(); };
	opTable[0][0x8] = [this]() {ld(); };

	opTable[0][0x3] = [this]() {inc(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0x3 | subset] = [this]() {inc(); };
	opTable[0][0xb] = [this]() {dec(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0xb | subset] = [this]() {dec(); };
	opTable[0][0x9] = [this]() {add(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0][0x9 | subset] = [this]() {add(); };

	opTable[0][0x4] = [this]() {inc(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0][0x4 | subset] = [this]() { inc(); };
	opTable[0][0x5] = [this]() {dec(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0][0x5 | subset] = [this]() { dec(); };

	opTable[0][0x6] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0][0x6 | subset] = [this]() { ld(); };

	opTable[0][0x7] = [this]() {rlca(); };
	opTable[0][0xf] = [this]() {rrca(); };
	opTable[0][0x17] = [this]() {rla(); };
	opTable[0][0x1f] = [this]() {rra(); };
	opTable[0][0x27] = [this]() {daa(); };
	opTable[0][0x2f] = [this]() {cpl(); };
	opTable[0][0x37] = [this]() {scf(); };
	opTable[0][0x3f] = [this]() {ccf(); };

	opTable[0][0x18] = [this]() {jr(); };
	opTable[0][0x20] = [this]() {jr(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[0][0x20 | subset] = [this]() { jr(); };

	opTable[0][0x10] = [this]() {stop(); };
#pragma endregion

#pragma region Block1
	// block 1
	opTable[1][0x40] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable[1][0x40 | subset] = [this]() { ld(); };
	opTable[1][0x76] = [this]() {halt(); };
#pragma endregion

#pragma region Block2
	// block 2
	opTable[2][0x80] = [this]() {add(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0x80 | subset] = [this]() { add(); };
	opTable[2][0x88] = [this]() {adc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0x88 | subset] = [this]() { adc(); };
	opTable[2][0x90] = [this]() {sub(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0x90 | subset] = [this]() { sub(); };
	opTable[2][0x98] = [this]() {sbc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0x98 | subset] = [this]() { sbc(); };
	opTable[2][0xa0] = [this]() {_and(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0xa0 | subset] = [this]() { _and(); };
	opTable[2][0xa8] = [this]() {_xor(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0xa8 | subset] = [this]() { _xor(); };
	opTable[2][0xb0] = [this]() {_or(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0xb0 | subset] = [this]() { _or(); };
	opTable[2][0xb8] = [this]() {cp(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[2][0xb8 | subset] = [this]() { cp(); };
#pragma endregion

#pragma region Block3
	// block 3
	opTable[3][0xc6] = [this]() {add(); };
	opTable[3][0xce] = [this]() {adc(); };
	opTable[3][0xd6] = [this]() {sub(); };
	opTable[3][0xde] = [this]() {sbc(); };
	opTable[3][0xe6] = [this]() {_and(); };
	opTable[3][0xee] = [this]() {_xor(); };
	opTable[3][0xf6] = [this]() {_or(); };
	opTable[3][0xfe] = [this]() {cp(); };

	opTable[3][0xc0] = [this]() {ret(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[3][0xc0 | subset] = [this]() { ret(); };
	opTable[3][0xc9] = [this]() {ret(); };
	opTable[3][0xd9] = [this]() {reti(); };
	opTable[3][0xc2] = [this]() {jp(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[3][0xc2 | subset] = [this]() { jp(); };
	opTable[3][0xc3] = [this]() {jp(); };
	opTable[3][0xe9] = [this]() {jp(); };
	opTable[3][0xc4] = [this]() {call(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[3][0xc4 | subset] = [this]() { call(); };
	opTable[3][0xcd] = [this]() {call(); };
	opTable[3][0xc7] = [this]() {rst(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[3][0xc7 | subset] = [this]() { rst(); };


	opTable[3][0xc1] = [this]() {pop(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[3][0xc1 | subset] = [this]() { pop(); };
	opTable[3][0xc5] = [this]() {push(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[3][0xc5 | subset] = [this]() { push(); };

	opTable[3][0xcb] = [this]() {cb$(); };// prefix

	opTable[3][0xe2] = [this]() {ldh(); };
	opTable[3][0xe0] = [this]() {ldh(); };
	opTable[3][0xea] = [this]() {ld(); };
	opTable[3][0xf2] = [this]() {ldh(); };
	opTable[3][0xf0] = [this]() {ldh(); };
	opTable[3][0xfa] = [this]() {ld(); };

	opTable[3][0xe8] = [this]() {add(); };
	opTable[3][0xf8] = [this]() {ld(); };
	opTable[3][0xf9] = [this]() {ld(); };

	opTable[3][0xf3] = [this]() {di(); };
	opTable[3][0xfb] = [this]() {ei(); };
#pragma endregion

#pragma region BlockCB
	// block cb
	opTable[4][0x0] = [this]() {rlc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x0 | subset] = [this]() { rlc(); };
	opTable[4][0x8] = [this]() {rrc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x8 | subset] = [this]() { rrc(); };
	opTable[4][0x10] = [this]() {rl(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x10 | subset] = [this]() { rl(); };
	opTable[4][0x18] = [this]() {rr(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x18 | subset] = [this]() { rr(); };
	opTable[4][0x20] = [this]() {sla(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x20 | subset] = [this]() { sla(); };
	opTable[4][0x28] = [this]() {sra(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x28 | subset] = [this]() { sra(); };
	opTable[4][0x30] = [this]() {swap(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x30 | subset] = [this]() { swap(); };
	opTable[4][0x38] = [this]() {srl(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[4][0x38 | subset] = [this]() { srl(); };

	opTable[4][0x40] = [this]() {bit(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable[4][0x40 | subset] = [this]() { bit(); };
	opTable[4][0x80] = [this]() {res(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable[4][0x80 | subset] = [this]() { res(); };
	opTable[4][0xc0] = [this]() {set(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable[4][0xc0 | subset] = [this]() { set(); };

#pragma endregion


}

void gbCPU::test()
{
	std::cout << opTable[0].size() << std::endl;
	std::cout << opTable[1].size() << std::endl;
	std::cout << opTable[2].size() << std::endl;
	std::cout << opTable[3].size() << std::endl;
	std::cout << opTable[4].size() << std::endl;

	for (const auto& pair : opTable[0]) {
		int key = pair.first;
		auto& func = pair.second;
		std::cout << "0x" << std::hex << key << " ";
		func();
	}
	for (const auto& pair : opTable[1]) {
		int key = pair.first;
		auto& func = pair.second;
		std::cout << "0x" << std::hex << key << " ";
		func();
	}

	for (const auto& pair : opTable[2]) {
		int key = pair.first;
		auto& func = pair.second;
		std::cout << "0x" << std::hex << key << " ";
		func();
	}

	for (const auto& pair : opTable[3]) {
		int key = pair.first;
		auto& func = pair.second;
		std::cout << "0x" << std::hex << key << " ";
		func();
	}

	for (const auto& pair : opTable[4]) {
		int key = pair.first;
		auto& func = pair.second;
		std::cout << "0x" << std::hex << key << " ";
		func();
	}

}

void gbCPU::tick()
{
}

void gbCPU::decodeOPcode()
{
	int8 op = currentOP;
	int8 block = GetBLOCK(op);
	switch (block)
	{
	case BLOCK0:
		break;
	case BLOCK1:
		break;
	case BLOCK2:
		break;
	case BLOCK3:
		break;
	case BLOCK$CB:
		break;

	default:
		break;
	}
}


// OP CODES
void gbCPU::nop()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::ld()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::inc()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::dec()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::add()
{
	std::cout << __func__ << std::endl;
	return; // temp
	int8 opType = GetBLOCK(currentOP); // get the block
	switch (opType)
	{
	case BLOCK0:
		// get the register by the bits (5 or 4)
		HL += *(r16[currentOP & 0x30]);
		break;
	case BLOCK2:
		// get the register by the bits (5 or 4)
		HL += *(r8[currentOP & 0x7]);
		break;
	case BLOCK3: // read 2 blocks
		int8 toAdd = nextOP;
		if (currentOP == 0xc6) // add to a
			A += toAdd;
		else if (currentOP == 0xc8) // add to sp
			SP += toAdd;
		break;
	}
}
void gbCPU::rlca()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rrca()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rla()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rra()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::daa()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::cpl()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::scf()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::ccf()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::jr()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::stop()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::halt()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::adc()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::sub()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::sbc()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::_and()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::_xor()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::_or()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::cp()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::ret()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::reti()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::jp()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::call()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rst()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::pop()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::push()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::ldh()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::di()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::ei()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::cb$()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rlc()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rrc()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rl()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::rr()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::sla()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::sra()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::swap()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::srl()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::bit()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::res()
{
	std::cout << __func__ << std::endl;
}
void gbCPU::set()
{
	std::cout << __func__ << std::endl;
}
