#pragma once
#include "Registers.h"
#include <string>

#define ROMBank0 0x3FFF
#define ROMBankN 0x7FFF
#define VRAM 0x9FFF
#define ExternalRAM 0xBFFF
#define WRAM 0xDFFF
#define EchoRAM 0xFDFF
#define OAM 0xFE9FF
#define Unused 0xFEFF
#define IO 0xFF7F
#define HRAM 0xFF7F
#define Interrupt 0xFFFF

class gbMM
{
private:
	int8 memory[0x10000] = { 0 };
public:
	void loadROM(const std::vector<uint8_t>& romData);
	int8 read8(int16 addr);
	void write8(int16 addr, int8 val);

	std::string currentName;
	bool mmTest(std::string fileN);
	bool mmTest2(std::string fileN);
	void printEnd(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp);
	bool compareRegs(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp);
	void printCPU();
};

