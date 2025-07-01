#include "gbCPU.h"
#include "emulator.h"
// temp
#include <iostream>
#include <fstream>
#define currentOP (emulator::MM.memory[PC])
#define currentOP16 ((emulator::MM.memory[PC]) | (emulator::MM.memory[PC+1] << 8))
#define proccedOP PC++;
#define proccedOP16 PC+=2;
#define nextOP (emulator::MM.memory[PC+1])
#define nextOP16 ((emulator::MM.memory[PC+1])+ (emulator::MM.memory[PC+2]<< 8))
#define getAddressMemory(addr) (emulator::MM.memory[addr])
#define setAddressMemory(addr, setNum) emulator::MM.memory[addr] = setNum;

#define FOR_BIT_SUBSETS(mask, subset) \
    for (int subset = (mask); subset != 0; subset = ((subset - 1) & (mask)))


gbCPU::gbCPU()
{
#pragma region Block0
	// block 0
	opTable[0x0] = [this]() {nop(); };

	opTable[0x1] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0x1 | subset] = [this]() {ld(); };
	opTable[0x2] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0x2 | subset] = [this]() {ld(); };
	opTable[0xa] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0xa | subset] = [this]() {ld(); };
	opTable[0x8] = [this]() {ld(); };

	opTable[0x3] = [this]() {inc(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0x3 | subset] = [this]() {inc(); };
	opTable[0xb] = [this]() {dec(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0xb | subset] = [this]() {dec(); };
	opTable[0x9] = [this]() {add(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0x9 | subset] = [this]() {add(); };

	opTable[0x4] = [this]() {inc(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0x4 | subset] = [this]() { inc(); };
	opTable[0x5] = [this]() {dec(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0x5 | subset] = [this]() { dec(); };

	opTable[0x6] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0x6 | subset] = [this]() { ld(); };

	opTable[0x7] = [this]() {rlca(); };
	opTable[0xf] = [this]() {rrca(); };
	opTable[0x17] = [this]() {rla(); };
	opTable[0x1f] = [this]() {rra(); };
	opTable[0x27] = [this]() {daa(); };
	opTable[0x2f] = [this]() {cpl(); };
	opTable[0x37] = [this]() {scf(); };
	opTable[0x3f] = [this]() {ccf(); };

	opTable[0x18] = [this]() {jr(); };
	opTable[0x20] = [this]() {jr(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[0x20 | subset] = [this]() { jr(); };

	opTable[0x10] = [this]() {stop(); };
#pragma endregion

#pragma region Block1
	// block 1
	opTable[0x40] = [this]() {ld(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable[0x40 | subset] = [this]() { ld(); };
	opTable[0x76] = [this]() {halt(); };
#pragma endregion

#pragma region Block2
	// block 2
	opTable[0x80] = [this]() {add(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0x80 | subset] = [this]() { add(); };
	opTable[0x88] = [this]() {adc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0x88 | subset] = [this]() { adc(); };
	opTable[0x90] = [this]() {sub(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0x90 | subset] = [this]() { sub(); };
	opTable[0x98] = [this]() {sbc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0x98 | subset] = [this]() { sbc(); };
	opTable[0xa0] = [this]() {_and(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0xa0 | subset] = [this]() { _and(); };
	opTable[0xa8] = [this]() {_xor(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0xa8 | subset] = [this]() { _xor(); };
	opTable[0xb0] = [this]() {_or(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0xb0 | subset] = [this]() { _or(); };
	opTable[0xb8] = [this]() {cp(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable[0xb8 | subset] = [this]() { cp(); };
#pragma endregion

#pragma region Block3
	// block 3
	opTable[0xc6] = [this]() {add(); };
	opTable[0xce] = [this]() {adc(); };
	opTable[0xd6] = [this]() {sub(); };
	opTable[0xde] = [this]() {sbc(); };
	opTable[0xe6] = [this]() {_and(); };
	opTable[0xee] = [this]() {_xor(); };
	opTable[0xf6] = [this]() {_or(); };
	opTable[0xfe] = [this]() {cp(); };

	opTable[0xc0] = [this]() {ret(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[0xc0 | subset] = [this]() { ret(); };
	opTable[0xc9] = [this]() {ret(); };
	opTable[0xd9] = [this]() {reti(); };
	opTable[0xc2] = [this]() {jp(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[0xc2 | subset] = [this]() { jp(); };
	opTable[0xc3] = [this]() {jp(); };
	opTable[0xe9] = [this]() {jp(); };
	opTable[0xc4] = [this]() {call(); };
	FOR_BIT_SUBSETS(0x18, subset)
		opTable[0xc4 | subset] = [this]() { call(); };
	opTable[0xcd] = [this]() {call(); };
	opTable[0xc7] = [this]() {rst(); };
	FOR_BIT_SUBSETS(0x38, subset)
		opTable[0xc7 | subset] = [this]() { rst(); };


	opTable[0xc1] = [this]() {pop(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0xc1 | subset] = [this]() { pop(); };
	opTable[0xc5] = [this]() {push(); };
	FOR_BIT_SUBSETS(0x30, subset)
		opTable[0xc5 | subset] = [this]() { push(); };

	opTable[0xcb] = [this]() {cb$(); };// prefix

	opTable[0xe2] = [this]() {ldh(); };
	opTable[0xe0] = [this]() {ldh(); };
	opTable[0xea] = [this]() {ld(); };
	opTable[0xf2] = [this]() {ldh(); };
	opTable[0xf0] = [this]() {ldh(); };
	opTable[0xfa] = [this]() {ld(); };

	opTable[0xe8] = [this]() {add(); };
	opTable[0xf8] = [this]() {ld(); };
	opTable[0xf9] = [this]() {ld(); };

	opTable[0xf3] = [this]() {di(); };
	opTable[0xfb] = [this]() {ei(); };
#pragma endregion

#pragma region BlockCB
	// block cb
	opTable$CB[0x0] = [this]() {rlc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x0 | subset] = [this]() { rlc(); };
	opTable$CB[0x8] = [this]() {rrc(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x8 | subset] = [this]() { rrc(); };
	opTable$CB[0x10] = [this]() {rl(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x10 | subset] = [this]() { rl(); };
	opTable$CB[0x18] = [this]() {rr(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x18 | subset] = [this]() { rr(); };
	opTable$CB[0x20] = [this]() {sla(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x20 | subset] = [this]() { sla(); };
	opTable$CB[0x28] = [this]() {sra(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x28 | subset] = [this]() { sra(); };
	opTable$CB[0x30] = [this]() {swap(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x30 | subset] = [this]() { swap(); };
	opTable$CB[0x38] = [this]() {srl(); };
	FOR_BIT_SUBSETS(0x7, subset)
		opTable$CB[0x38 | subset] = [this]() { srl(); };

	opTable$CB[0x40] = [this]() {bit(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable$CB[0x40 | subset] = [this]() { bit(); };
	opTable$CB[0x80] = [this]() {res(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable$CB[0x80 | subset] = [this]() { res(); };
	opTable$CB[0xc0] = [this]() {set(); };
	FOR_BIT_SUBSETS(0x3f, subset)
		opTable$CB[0xc0 | subset] = [this]() { set(); };

#pragma endregion

}

void gbCPU::test()
{
	std::cout << "===================== enter cpu =====================\n";
	tick();
	std::cout << "===================== exit cpu ======================\n";
}

void gbCPU::tick()
{
	// timer
	decodeOPcode();
}

int gbCPU::getRegJson(int c)
{
	switch (c)
	{
	case 'a':
		return AA;
		break;
	case 'b':
		return BB;
		break;
	case 'c':
		return CC;
		break;
	case 'd':
		return DD;
		break;
	case 'e':
		return EE;
		break;
	case 'f':
		return FF;
		break;
	case 'h':
		return HH;
		break;
	case 'l':
		return LL;
		break;
	case 'p':
		return PC;
		break;
	case 's':
		return SP;
		break;

	default:
		std::cout << "no get reg";
		break;
	}
	return 0;
}

void gbCPU::setRegJson(int c, int setNum)
{
	switch (c)
	{
	case 'a':
		AA = setNum;
		break;
	case 'b':
		BB = setNum;
		break;
	case 'c':
		CC = setNum;
		break;
	case 'd':
		DD = setNum;
		break;
	case 'e':
		EE = setNum;
		break;
	case 'f':
		FF = setNum;
		break;
	case 'h':
		HH = setNum;
		break;
	case 'l':
		LL = setNum;
		break;
	case 'p':
		PC = setNum;
		break;
	case 's':
		SP = setNum;
		break;

	default:
		std::cout << "no reg";
		break;
	}
}

void gbCPU::cb$()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	int8 op = currentOP;
	opTable$CB[op]();
}

void gbCPU::decodeOPcode()
{
	int8 op = currentOP;
	if (opTable.contains(op))
		opTable[op]();
	else
	{
		std::ofstream("OpCodelog.txt", std::ios::app) << "no OP code: 0x" << std::hex << +op << " func> " << emulator::MM.currentName << "\n";
	}
}

int8 gbCPU::getConditionOp(int8 cond)
{
	switch (cond)
	{
	case 0x0:
		return !getZero;
		break;
	case 0x1:
		return getZero;
		break;
	case 0x2:
		return !getCarry;
		break;
	case 0x3:
		return getCarry;
		break;
	}
}

void gbCPU::set8RegisterOp(int8 reg, int8 setNum)
{
	switch (reg)
	{
	case 0x0:
		BB = setNum;
		break;
	case 0x1:
		CC = setNum;
		break;
	case 0x2:
		DD = setNum;
		break;
	case 0x3:
		EE = setNum;
		break;
	case 0x4:
		HH = setNum;
		break;
	case 0x5:
		LL = setNum;
		break;
	case 0x6:
		setAddressMemory(HL, setNum);
		break;
	case 0x7:
		AA = setNum;
		break;
	default:
		std::cout << "error 8 bit reg set\n";
		break;
	}
}
int8 gbCPU::get8RegisterOp(int8 reg)
{

	switch (reg)
	{
	case 0x0:
		return BB;
	case 0x1:
		return CC;
	case 0x2:
		return DD;
	case 0x3:
		return EE;
	case 0x4:
		return HH;
	case 0x5:
		return LL;
	case 0x6:
		return getAddressMemory(HL);
	case 0x7:
		return AA;
	}
}

void gbCPU::set16RegisterOp(int8 reg, int16 setNum)
{
	switch (reg)
	{
	case 0x0:
		BC = setNum;
		break;
	case 0x1:
		DE = setNum;
		break;
	case 0x2:
		HL = setNum;
		break;
	case 0x3:
		SP = setNum;
		break;
	default:
		std::cout << "error 16 bit reg set\n";
		break;
	}
}
int16 gbCPU::get16RegisterOp(int8 reg)
{
	switch (reg)
	{
	case 0x0:
		return BC;
	case 0x1:
		return DE;
	case 0x2:
		return HL;
	case 0x3:
		return SP;
	}
}

void gbCPU::setRegisterMemoryOp(int8 mem, int8 setNum)
{
	std::cout << std::hex << +mem << "\n";
	std::cout << std::hex << +setNum << "\n";
	switch (mem)
	{
	case 0x0:

		setAddressMemory(BC, setNum);
		break;
	case 0x1:
		setAddressMemory(DE, setNum);
		break;
	case 0x2:
		setAddressMemory(HL++, setNum);
		break;
	case 0x3:
		setAddressMemory(HL--, setNum);
		break;
	default:
		std::cout << "error reg mem set\n";
		break;
	}
}
int8 gbCPU::getRegisterMemoryOp(int8 mem)
{
	switch (mem)
	{
	case 0x0:
		return getAddressMemory(BC);
	case 0x1:
		return getAddressMemory(DE);
	case 0x2:
		return getAddressMemory(HL++);
	case 0x3:
		return getAddressMemory(HL--);
	}
}

// OP CODES
void gbCPU::nop()
{
	// do nothing?... i guess
	// only used for the timmer and cycele things. will be implemented later
	proccedOP;
	std::cout << __func__ << std::endl;
}
void gbCPU::ld()
{
	std::cout << __func__ << std::endl;
	int16 immOP16 = 0; // avoid warrnings
	int8 immOP8 = 0; // avoid warrnings
	int8 op = currentOP;
	proccedOP;
	int8 block = GetBLOCK(op);
	switch (block)
	{
	case BLOCK0:
		if ((op & 0b110) == 0b110) // ld r8, imm8
		{
			immOP8 = currentOP;
			proccedOP;
			set8RegisterOp(tools::getSumRightRotateMask(op, 3, 0b111), immOP8);
		}
		else
		{
			switch (op & 0b1111)
			{
			case 0b0001: // ld r16, imm16
				immOP16 = currentOP16;
				proccedOP16;
				set16RegisterOp(tools::getSumRightRotateMask(op, 4, 0b11), immOP16);
				break;
			case 0b0010: // ld [r16mem], a
				std::cout << std::hex << +AA << "\n";
				setRegisterMemoryOp(tools::getSumRightRotateMask(op, 4, 0b11), AA);
				break;
			case 0b1010: // ld a, [r16mem]
				AA = getRegisterMemoryOp(tools::getSumRightRotateMask(op, 4, 0b11));
				break;
			case 0b1000: // ld [imm16], sp
				int16 addr = currentOP16;
				proccedOP16;
				setAddressMemory(addr, SP);
				break;
			}
		}
		break;
	case BLOCK1:
		// ld r8, r8
		if (op != 0b01110110) // not halt command
			set8RegisterOp(tools::getSumRightRotateMask(op, 3, 0b111), get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111)));
		break;
	case BLOCK3:
		int16 addr = 0;
		int8 toAdd = 0;
		switch (op)
		{
		case 0b11101010: // ld [imm16], a
			addr = currentOP16;
			proccedOP16;
			setAddressMemory(addr, AA);
			break;
		case 0b11111010: // ld a, [imm16]
			addr = currentOP16;
			proccedOP16;
			AA = getAddressMemory(addr);
			break;
		case 0b11111000: // ld hl, sp + imm8
			setZero(0);
			setN(0);
			toAdd = currentOP;
			proccedOP;
			setHCarry(((SP & 0x0f) + (toAdd & 0x0f)) > 0x0f);
			setHCarry(((SP & 0xff) + (toAdd & 0xff)) > 0xff);
			HL = SP + toAdd;
			break;
		case 0b11111001: // ld sp, hl
			SP = HL;
			break;
		}
		break;

	}
}
void gbCPU::inc()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	if ((op & 0b011) == 0b011) // inc r16
	{
		int8 regPos = tools::getSumRightRotateMask(op, 4, 0b11);
		set16RegisterOp(regPos, get16RegisterOp(regPos) + 1);
	}
	else if ((op & 0b100) == 0b100) // inc r8
	{
		int8 regPos = tools::getSumRightRotateMask(op, 3, 0b111);
		int8 toAdd = get8RegisterOp(regPos);
		setHCarry(((toAdd & 0x0f) + 1) > 0x0f);
		toAdd++;
		setZero(!toAdd);
		setN(0);
		set8RegisterOp(regPos, toAdd);
	}
}
void gbCPU::dec()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	if ((op & 0b1011) == 0b1011) // dec r16
	{
		int8 regPos = tools::getSumRightRotateMask(op, 4, 0b11);
		set16RegisterOp(regPos, get16RegisterOp(regPos) - 1);
	}
	else if ((op & 0b101) == 0b101) // dec r8
	{
		int8 regPos = tools::getSumRightRotateMask(op, 3, 0b111);
		int8 toSub = get8RegisterOp(regPos);
		setHCarry((toSub & 0x0f) == 0);
		toSub--;
		setZero(!toSub);
		setN(1);
		set8RegisterOp(regPos, toSub);
	}
}
void gbCPU::add()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	int8 opType = GetBLOCK(op); // get the block

	int16 toAdd16 = 0; // avoid warrnings
	int8 toAdd8 = 0; // avoid warrnings
	switch (opType)
	{
	case BLOCK0:
		// add hl, r16
		toAdd16 = get16RegisterOp(tools::getSumRightRotateMask(op, 4, 0b11));
		setCarry((HL + toAdd16) > 0xffff);
		setHCarry(((HL & 0x0fff) + (toAdd16 & 0x0fff)) > 0x0fff);
		HL += toAdd16;
		break;
	case BLOCK2:
		// add a, r8
		toAdd8 = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
		setCarry((AA + toAdd8) > 0xff);
		setHCarry(((AA & 0x0f) + (toAdd8 & 0x0f)) > 0x0f);
		AA += toAdd8;
		setZero(!AA);
		break;
	case BLOCK3:
		toAdd8 = currentOP;
		proccedOP;
		if (op == 0xc6) // add a, imm8
		{
			setCarry((AA + toAdd8) > 0xff);
			setHCarry(((AA & 0x0f) + (toAdd8 & 0x0f)) > 0x0f);
			AA += toAdd8;
			setZero(!AA);
		}
		else if (op == 0xc8) // add sp, imm8
		{
			setZero(0);
			setCarry(((SP & 0xff) + toAdd8) > 0xff);
			setHCarry(((SP & 0x0f) + (toAdd8 & 0x0f)) > 0x0f);
			SP += toAdd8;
		}
		break;
	}
}
void gbCPU::halt()
{
	// i guess do nothing for now too... untill timer and interrupt is implemented
	proccedOP;
	std::cout << __func__ << std::endl;
}
void gbCPU::adc()
{
	std::cout << __func__ << std::endl;
	int8 carry = getCarry;
	int8 toAdd = 0;
	int8 op = currentOP;
	proccedOP;

	if (op == 0xce) // adc a, imm8
	{
		toAdd = currentOP;
		proccedOP;
	}
	else // adc a, r8
	{
		toAdd = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	setCarry((AA + toAdd + carry) > 0xff);
	setHCarry(((AA & 0x0f) + (toAdd & 0x0f) + carry) > 0x0f);
	AA += toAdd + carry;
	setZero(!AA);
	setN(0);
}
void gbCPU::sub()
{
	std::cout << __func__ << std::endl;
	int8 toSub = 0;
	int8 op = currentOP;
	proccedOP;
	setN(1);
	if (op == 0xd6) // sub a, imm8
	{
		toSub = currentOP;
		proccedOP;
	}
	else // sub a, r8
	{
		toSub = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	setCarry(AA < toSub);
	setHCarry((AA & 0x0f) < (toSub & 0x0f));
	AA -= toSub;
	setZero(!AA);
}
void gbCPU::sbc()
{
	std::cout << __func__ << std::endl;
	int8 carry = getCarry;
	int8 toSub = 0;
	int8 op = currentOP;
	proccedOP;
	setN(1);
	if (op == 0xd6) // sub a, imm8
	{
		toSub = currentOP;
		proccedOP;
	}
	else // sub a, r8
	{
		toSub = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	setCarry(AA < (toSub + carry));
	setHCarry((AA & 0x0f) < ((toSub & 0x0f) + carry));
	AA -= (toSub + carry);
	setZero(!AA);
}
void gbCPU::_and()
{
	std::cout << __func__ << std::endl;
	int8 toComp = 0;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(1);
	setCarry(0);
	if (op == 0xe6) // and a, imm8
	{
		toComp = currentOP;
		proccedOP;
	}
	else // and a, r8
	{
		toComp = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	AA = AA & toComp;
	setZero(!AA);
}
void gbCPU::_xor()
{
	std::cout << __func__ << std::endl;
	int8 toComp = 0;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);
	setCarry(0);
	if (op == 0xee) // xor a, imm8
	{
		toComp = currentOP;
		proccedOP;
	}
	else // xor a, r8
	{
		toComp = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	AA = AA ^ toComp;
	setZero(!AA);
}
void gbCPU::_or()
{
	std::cout << __func__ << std::endl;
	int8 toComp = 0;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);
	setCarry(0);
	if (op == 0xf6) // or a, imm8
	{
		toComp = currentOP;
		proccedOP;
	}
	else // or a, r8
	{
		toComp = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	AA = AA | toComp;
	setZero(!AA);
}
void gbCPU::cp()
{
	std::cout << __func__ << std::endl;
	int8 toComp = 0;
	int8 op = currentOP;
	proccedOP;
	setN(1);
	if (op == 0xf6) // cp a, imm8
	{
		toComp = currentOP;
		proccedOP;
	}
	else // cp a, r8
	{
		toComp = get8RegisterOp(tools::getSumRightRotateMask(op, 0, 0b111));
	}
	setCarry(AA < toComp);
	setHCarry((AA & 0x0f) < (toComp & 0x0f));
	setZero(AA == toComp);
}
void gbCPU::pop()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;

	int8 low = getAddressMemory(SP++);
	int8 up = getAddressMemory(SP++);

	switch (op)
	{
	case 0xc1:
		BB = up;
		CC = low;
		break;
	case 0xd1:
		DD = up;
		EE = low;
		break;
	case 0xe1:
		HH = up;
		LL = low;
		break;
	case 0xf1:
		AA = up;
		FF = low;
		break;
	}
}
void gbCPU::push()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	SP--;
	switch (op)
	{
	case 0xc5:
		setAddressMemory(SP--, BB);
		setAddressMemory(SP, CC);
		break;
	case 0xd5:
		setAddressMemory(SP--, DD);
		setAddressMemory(SP, EE);
		break;
	case 0xe5:
		setAddressMemory(SP--, HH);
		setAddressMemory(SP, LL);
		break;
	case 0xf5:
		setAddressMemory(SP--, AA);
		setAddressMemory(SP, FF);
		break;
	}
}
void gbCPU::swap()
{
	std::cout << __func__ << std::endl;
	// swap r8
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);
	setCarry(0);
	int8 regPos = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regPos);
	reg = ((reg & 0x0f) << 4) | ((reg & 0xf0) >> 4);
	set8RegisterOp(regPos, reg);
	setZero(!reg);
}
void gbCPU::cpl()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setN(1);
	setHCarry(1);
	AA = ~AA;
}
void gbCPU::stop()
{
	proccedOP;
	proccedOP;
	std::cout << __func__ << std::endl;
}
void gbCPU::daa()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	int8 toAdjust = 0;

	if (getN)
	{
		if (getHCarry)
			toAdjust += 0x6;
		if (getCarry)
			toAdjust += 0x60;
		AA -= toAdjust;
	}
	else
	{
		if (getHCarry || ((AA & 0x0f) > 9))
			toAdjust += 0x6;

		if (getCarry || (AA > 0x99))
		{
			toAdjust += 0x60;
			setCarry(1);
		}
		AA += toAdjust;
	}
	setHCarry(0);
	setZero(!AA);
}
void gbCPU::di()
{
	std::cout << __func__ << std::endl;
	// nothing to do untill interrupts complete
	proccedOP;
	std::cout << __func__ << std::endl;
}
void gbCPU::ei()
{
	std::cout << __func__ << std::endl;
	// nothing to do untill interrupts complete
	proccedOP;
	std::cout << __func__ << std::endl;
}
void gbCPU::scf()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setN(0);
	setHCarry(0);
	setCarry(1);
}
void gbCPU::ccf()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setN(0);
	setHCarry(0);
	setCarry(!getCarry);
}
void gbCPU::rlca()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setZero(0);
	setN(0);
	setHCarry(0);

	setCarry(AA & 0x80);
	AA = (AA << 1) | ((AA & 0x80) >> 7);
}
void gbCPU::rrca()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setZero(0);
	setN(0);
	setHCarry(0);

	setCarry(AA & 0x01);
	AA = (AA >> 1) | ((AA & 0x01) << 7);
}
void gbCPU::rla()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setZero(0);
	setN(0);
	setHCarry(0);

	int8 carry = getCarry;
	setCarry(AA & 0x80);
	AA = (AA << 1) | carry;

}
void gbCPU::rra()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	setZero(0);
	setN(0);
	setHCarry(0);

	int8 carry = getCarry;
	setCarry(AA & 0x01);
	AA = (AA >> 1) | (carry << 7);
}
void gbCPU::rlc()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	setCarry(reg & 0x80);
	reg = (reg << 1) | ((reg & 0x80) >> 7);
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::rl()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	int8 carry = getCarry;
	setCarry(reg & 0x80);
	reg = (reg << 1) | (carry);
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::rrc()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	setCarry(reg & 0x01);
	reg = (reg >> 1) | ((reg & 0x01) << 7);
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::rr()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	int8 carry = getCarry;
	setCarry(reg & 0x01);
	reg = (reg >> 1) | (carry << 7);
	set8RegisterOp(regOP, reg);
	setZero(!reg);

}
void gbCPU::sla()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	setCarry(reg & 0x80);
	reg = reg << 1;
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::sra()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	setCarry(reg & 0x01);
	reg = (reg >> 1) | (reg & 0x80);
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::srl()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(0);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 reg = get8RegisterOp(regOP);
	setCarry(reg & 0x01);
	reg = (reg >> 1);
	set8RegisterOp(regOP, reg);
	setZero(!reg);
}
void gbCPU::bit()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	setN(0);
	setHCarry(1);

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 bitInd = tools::getSumRightRotateMask(op, 3, 0b111);
	int8 reg = get8RegisterOp(regOP);

	setZero(!(reg & (1 << bitInd)));
}
void gbCPU::res()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 bitInd = tools::getSumRightRotateMask(op, 3, 0b111);
	int8 reg = get8RegisterOp(regOP);

	reg = reg & (~(1 << bitInd));
	set8RegisterOp(regOP, reg);
}
void gbCPU::set()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;

	int8 regOP = tools::getSumRightRotateMask(op, 0, 0b111);
	int8 bitInd = tools::getSumRightRotateMask(op, 3, 0b111);
	int8 reg = get8RegisterOp(regOP);

	reg = reg | (1 << bitInd);
	set8RegisterOp(regOP, reg);
}
void gbCPU::ret()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;

	bool retFunc = false;
	if (op == 0b11001001) // ret
		retFunc = true;
	else // ret cond
		retFunc = (getConditionOp(tools::getSumRightRotateMask(op, 3, 0b11)));

	if (retFunc)
	{
		int16 addr = 0;
		addr = getAddressMemory(SP++);
		addr = addr | (getAddressMemory(SP++) << 8);
		PC = addr;
	}
}
void gbCPU::reti()
{
	std::cout << __func__ << std::endl;
	proccedOP;
	int16 addr = 0;
	addr = getAddressMemory(SP++);
	addr = addr | (getAddressMemory(SP++) << 8);
	PC = addr;
	// now it should enable interrupts
}
void gbCPU::jp()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;

	int16 addr = 0;
	switch (op)
	{
	case 0b11000011: // jp imm16
		addr = currentOP16;
		proccedOP16;
		PC = addr;
		break;
	case 0b11101001: // jp hl
		PC = HL;
		break;
	default:
		// jp cond, imm16
		addr = currentOP16;
		proccedOP16;
		if (getConditionOp(tools::getSumRightRotateMask(op, 3, 0b11)))
			PC = addr;
		break;
	}
}
void gbCPU::jr()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	int8_t offset = currentOP;
	proccedOP;

	if (op == 0b11000) // jr imm8
	{
		PC += offset;
	}
	else // jr cond, imm8
	{
		if (getConditionOp(tools::getSumRightRotateMask(op, 3, 0b11)))
			PC += offset;
	}
}
void gbCPU::call()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	int16 addr = currentOP16;
	proccedOP16;
	bool callFunc = false;

	if (op == 0b11001101) // call imm16
		callFunc = true;
	else // call cond, imm16
		callFunc = (getConditionOp(tools::getSumRightRotateMask(op, 3, 0b11)));

	if (callFunc)
	{
		setAddressMemory(--SP, (PC >> 8) & 0xff);
		setAddressMemory(--SP, PC & 0xff);
		PC = addr;
	}
}
void gbCPU::rst()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	int16 addr = op & 0b00111000;

	setAddressMemory(--SP, (PC >> 8) & 0xff);
	setAddressMemory(--SP, PC & 0xff);
	PC = addr;
}
void gbCPU::ldh()
{
	std::cout << __func__ << std::endl;
	int8 op = currentOP;
	proccedOP;
	int16 addr = 0;

	switch (op)
	{
	case 0b11100010: // ldh [c], a
		setAddressMemory(0xff00 + CC, AA);
		break;
	case 0b11100000: // ldh [imm8], a
		addr = currentOP;
		proccedOP;
		setAddressMemory(0xff00 + addr, AA);
		break;
	case 0b11110010: // ldh a, [c]
		AA = getAddressMemory(0xff00 + CC);
		break;
	case 0b11110000: // ldh a, [imm8]
		addr = currentOP;
		proccedOP;
		AA = getAddressMemory(0xff00 + addr);
		break;
	}
}