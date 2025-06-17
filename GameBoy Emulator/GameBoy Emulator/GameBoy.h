#pragma once
#include "GbCPU.h"
#include "GbMMU.h"

class GameBoy
{
public:
	static GbCPU cpu;
	static GbMMU mmu;
};

