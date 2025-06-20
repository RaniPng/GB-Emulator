#pragma once
#include <unordered_map>
#include <functional>
#include "Registers.h"
#include "tools.h"

// zero flag - set if the last calculation returned zero 
#define ZFLAG 0x40
// n flag - indicates whether the previous instruction has been a subtraction
#define NFLAG 0x20
// half carry flag - carry flag but for the 4 lower bits
#define HFLAG 0x10
// carry flag - carry flag for the whole 8 bits
#define CFLAG 0x8

/*flag macros*/
// set
#define setZero F |= ZFLAG;
#define setN F |= NFLAG;
#define setHCarry F |= HFLAG;
#define setCarry F |= CFLAG;
// clear
#define clearFlags F ^= F;
#define clearZero F &= ~ZFLAG;
#define clearN F &= ~NFLAG;
#define clearHCarry F &= ~HFLAG;
#define clearCarry F &= ~CFLAG;
// get
#define getZero (F & ZFLAG)
#define getN (F & NFLAG)
#define getHCarry (F & HFLAG)
#define getCarry (F & CFLAG)

class gbCPU
{
public:
	gbCPU();
private:
	/*Registers*/
	// AF - A (high) is used for all the calculations and transformations, while F is used to indicate the flags
	Register _AF;
#define AF (_AF.full)
#define A (_AF.high)
#define F (_AF.low)
	Register _BC;
#define BC (_BC.full)
#define B (_BC.high)
#define C (_BC.low)
	Register _DE;
#define DE (_DE.full)
#define D (_DE.high)
#define E (_DE.low)
	Register _HL;
#define HL (_HL.full)
#define H (_HL.high)
	// L define is in used
#define LL (_HL.low)
	/*pointers*/
	int16 SP; // "pointer" to the stack current address
	int16 PC; // "pointer" to the next reading line / pointer


public: // temp~~~~~~
	void test();
	void tick();
	void decodeOPcode();
	void execiteInstruction(int8 op);
	void fetch();
	// op codes funcs
	std::unordered_map<int8, std::function<void()>> opTable[5];
	void nop();
	void ld();
	void inc();
	void dec();
	void add();
	void rlca();
	void rrca();
	void rla();
	void rra();
	void daa();
	void cpl();
	void scf();
	void ccf();
	void jr();
	void stop();
	void halt();
	void adc();
	void sub();
	void sbc();
	void _and();
	void _xor();
	void _or();
	void cp();
	void ret();
	void reti();
	void jp();
	void call();
	void rst();
	void pop();
	void push();
	void ldh();
	void di();
	void ei();

	void cb$();
	void rlc();
	void rrc();
	void rl();
	void rr();
	void sla();
	void sra();
	void swap();
	void srl();
	void bit();
	void res();
	void set();

	std::unordered_map<int8, int8*> r8;
	std::unordered_map<int8, int16*> r16;
	std::unordered_map<int8, int16*> r16stk;
	std::unordered_map<int8, int16*> r16mem; // might need to use lamda
	std::unordered_map<int8, std::function<int()>> condition; // might better to use lamda 

};

/*opcode block*/
#define BLOCK0 0x0
#define BLOCK1 0x40
#define BLOCK2 0x80
#define BLOCK3 0xc0
#define BLOCK$CB 0xcb
#define GetBLOCK(num) (num & 0xc0)
