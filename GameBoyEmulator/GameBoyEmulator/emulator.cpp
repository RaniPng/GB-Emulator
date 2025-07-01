#include "emulator.h"

#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

gbCPU emulator::CPU;
gbMM emulator::MM;


void emulator::run()
{
	std::string folder = "testCPU/v1";
	for (const auto& entry : fs::directory_iterator(folder)) {
		if (entry.is_regular_file()) {
			std::cout << "File: " << entry.path() << "\n";
			MM.mmTest(entry.path().string());
		}
	}
	std::cout << "finished!! :)\n";
}
