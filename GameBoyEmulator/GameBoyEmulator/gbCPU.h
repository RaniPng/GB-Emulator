#pragma once
#include <unordered_map>
#include <functional>
#include "Registers.h"
#include "tools.h"
#include <map>

// zero flag - set if the last calculation returned zero 
#define ZFLAG 0x80
// n flag - indicates whether the previous instruction has been a subtraction
#define NFLAG 0x40
// half carry flag - carry flag but for the 4 lower bits
#define HFLAG 0x20
// carry flag - carry flag for the whole 8 bits
#define CFLAG 0x10

/*flag macros*/
// set
#define setZero(b)		FF = (b)? (FF | ZFLAG): (FF & (~ZFLAG));
#define setN(b)			FF = (b)? (FF | NFLAG): (FF & (~NFLAG));
#define setHCarry(b)	FF = (b)? (FF | HFLAG): (FF & (~HFLAG));
#define setCarry(b)		FF = (b)? (FF | CFLAG): (FF & (~CFLAG));
// get
#define getZero (!!(FF & ZFLAG))
#define getN (!!(FF & NFLAG))
#define getHCarry (!!(FF & HFLAG))
#define getCarry (!!(FF & CFLAG))


class gbCPU
{
public:
	gbCPU();
private:
	/*Registers*/
	// AF - A (high) is used for all the calculations and transformations, while F is used to indicate the flags
	Register _AF;
#define AF (_AF.full)
#define AA (_AF.high)
#define FF (_AF.low)
	Register _BC;
#define BC (_BC.full)
#define BB (_BC.high)
#define CC (_BC.low)
	Register _DE;
#define DE (_DE.full)
#define DD (_DE.high)
#define EE (_DE.low)
	Register _HL;
#define HL (_HL.full)
#define HH (_HL.high)
#define LL (_HL.low)
	/*pointers*/
	int16 SP; // "pointer" to the stack current address
	int16 PC; // "pointer" to the next reading line / pointer


public: // temp~~~~~~
	void test();
	void tick();
	int getRegJson(int c);
	void setRegJson(int c, int setNum);
private:
	void decodeOPcode();
	int8 getConditionOp(int8 cond);
	void set8RegisterOp(int8 reg, int8 setNum);
	int8 get8RegisterOp(int8 reg);
	void set16RegisterOp(int8 reg, int16 setNum);
	int16 get16RegisterOp(int8 reg);
	void setRegisterMemoryOp(int8 mem, int8 setNum);
	int8 getRegisterMemoryOp(int8 mem);
	void execiteInstruction(int8 op);
	void fetch();
	// op codes funcs
	//std::unordered_map<int8, std::function<void()>> opTable[5];
	//std::map<int8, std::function<void()>> opTable[5];
	std::map<int8, std::function<void()>> opTable;
	std::map<int8, std::function<void()>> opTable$CB;
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

	std::unordered_map<int8, std::function<int()>> condition; // might better to use lamda 
};

/*opcode block*/
#define BLOCK0 0x0
#define BLOCK1 0x40
#define BLOCK2 0x80
#define BLOCK3 0xc0
#define BLOCK$CB 0xcb
#define GetBLOCK(num) (num & 0xc0)
