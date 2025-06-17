#pragma once
#include "registers.h"

// Zero flag
#define Zf 0x80
// Prvius instruction has been substraction
#define Nf 0x40
// Indicates carry for the lower 4 bits of the result
#define Hf 0x20
// Carry flag
#define Cf 0x10

class GbCPU
{
private:
	/*registers*/
	// AF
	Register _AF; // Register F has a value that declare the state of the flags (according to it bits)
#define A (_AF.high)
#define F (_AF.low)
#define AF (_AF.full)
	// BC
	Register _BC;
#define B (_BC.high)
#define C (_BC.low)
#define BC (_BC.full)
	// DE
	Register _DE;
#define D (_DE.high)
#define E (_DE.low)
#define DE (_DE.full)
	// HL
	Register HL_R;
#define H (HL_R.high)
#define LL (HL_R.low)
#define HL (HL_R.full)
	// Pointers
	int16 SP; // stack pointer
	int16 PC; // command pointer

public: // temp
	void temp();
	void fetchOpcode(); // read the next opcode and detirmine what to do
	void tick(); // game tick/update (next gpu calculation)
};

/*Manipulate F*/
// Carry defines
#define isCarry (F & Cf)
#define setCarry F |= Cf;
#define clearCarry F &= ~Cf;
// Zero defines
#define isZero (F & Zf)
#define setZero F |= Zf;
#define clearZero F &= ~Zf;
// NF defines
#define isN (F & Nf)
#define setN F |= Nf;
#define clearN F &= ~Nf;
// HF defines
#define isH (F & Hf)
#define setH F |= Hf;
#define clearH F &= ~Hf;

	/*commands*/
#define Block0 0x00
#define Block1 0x40
#define Block2 0x80
#define Block3 0xc0
#define Block$CB 0x00
#define getCommandBlock(x) (x & 0xc0)
#define isCommandBlock(block, x) (getCommandBlock(x) == block)

enum Commands
{
	nop = 0x0
	, add = 20
};
