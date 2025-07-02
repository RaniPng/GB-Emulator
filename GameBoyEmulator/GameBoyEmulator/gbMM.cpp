#include "gbMM.h"
#include "emulator.h"

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


uint16_t parse_hex(const std::string& hex_str) {
	return static_cast<uint16_t>(std::stoi(hex_str, nullptr, 16));
}

int8 gbMM::read8(int16 addr)
{
	return memory[addr];
}

void gbMM::write8(int16 addr, int8 val)
{
	memory[addr] = val;
}

bool gbMM::mmTest(std::string fileN)
{
	std::remove("OpCodelog.txt");
	std::ifstream file(fileN);
	if (!file) {
		std::cerr << "Could not open tests.json" << std::endl;
		return false;
	}

	json test_cases;
	file >> test_cases;
	int i = 1;
	// Iterate over each test case in the array
	for (const auto& test : test_cases)
	{
		// keep output of the wrong calculates
		std::ostringstream buffer;
		std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

		std::cout << "==========================================================================\n";
		std::cout << "Running test: " << test["name"] << "\n";
		currentName = test["name"];
		// Initial CPU values
		auto intVal = test["initial"]["cpu"];
		//emulator::CPU
		emulator::CPU.setRegJson('a', intVal.contains("a") ? parse_hex(intVal["a"]) : 0);
		emulator::CPU.setRegJson('b', intVal.contains("b") ? parse_hex(intVal["b"]) : 0);
		emulator::CPU.setRegJson('c', intVal.contains("c") ? parse_hex(intVal["c"]) : 0);
		emulator::CPU.setRegJson('d', intVal.contains("d") ? parse_hex(intVal["d"]) : 0);
		emulator::CPU.setRegJson('e', intVal.contains("e") ? parse_hex(intVal["e"]) : 0);
		emulator::CPU.setRegJson('f', intVal.contains("f") ? parse_hex(intVal["f"]) : 0);
		emulator::CPU.setRegJson('h', intVal.contains("h") ? parse_hex(intVal["h"]) : 0);
		emulator::CPU.setRegJson('l', intVal.contains("l") ? parse_hex(intVal["l"]) : 0);
		emulator::CPU.setRegJson('p', intVal.contains("pc") ? parse_hex(intVal["pc"]) : 0);
		emulator::CPU.setRegJson('s', intVal.contains("sp") ? parse_hex(intVal["sp"]) : 0);

		std::cout << "Initial CPU:\n";
		printCPU();
		std::cout << "------------------------\n";

		// RAM
		std::cout << "RAM:\n";
		if (test["initial"].contains("ram")) {
			for (const auto& entry : test["initial"]["ram"]) {
				if (entry.size() == 2) {
					uint16_t addr = parse_hex(entry[0]);
					uint8_t val = parse_hex(entry[1]);
					memory[addr] = val;

					if (memory[addr])
						std::cout << "  [" << std::hex << addr << "] = 0x" << +memory[addr] << "\n";
				}
			}
		}

		// make cpu run
		emulator::CPU.test();

		// Final CPU values
		auto final_cpu = test["final"]["cpu"];
		int8 fa = final_cpu.contains("a") ? parse_hex(final_cpu["a"]) : 0;
		int8 fb = final_cpu.contains("b") ? parse_hex(final_cpu["b"]) : 0;
		int8 fc = final_cpu.contains("c") ? parse_hex(final_cpu["c"]) : 0;
		int8 fd = final_cpu.contains("d") ? parse_hex(final_cpu["d"]) : 0;
		int8 fe = final_cpu.contains("e") ? parse_hex(final_cpu["e"]) : 0;
		int8 ff = final_cpu.contains("f") ? parse_hex(final_cpu["f"]) : 0;
		int8 fh = final_cpu.contains("h") ? parse_hex(final_cpu["h"]) : 0;
		int8 fl = final_cpu.contains("l") ? parse_hex(final_cpu["l"]) : 0;
		int16 fpc = final_cpu.contains("pc") ? parse_hex(final_cpu["pc"]) : 0;
		int16 fsp = final_cpu.contains("sp") ? parse_hex(final_cpu["sp"]) : 0;

		bool rightAnswer = compareRegs(fa, fb, fc, fd, fe, ff, fh, fl, fpc, fsp);

		std::cout << "------------------------\n";
		// reset mem
		if (test["initial"].contains("ram"))
			for (const auto& entry : test["initial"]["ram"])
				if (entry.size() == 2) {
					int16 addr = parse_hex(entry[0]);
					memory[addr] = 0;
				}

		// reset cout
		std::cout.rdbuf(old); // Restore cout
		if (!rightAnswer) {
			std::cout << buffer.str();
			return false;
			//exit(-1);
			if (i++ == 2)
				return false;
		}
	}
	return true;
}

bool gbMM::mmTest2(std::string fileN)
{
	std::remove("OpCodelog.txt");
	std::ifstream file(fileN);
	if (!file) {
		std::cerr << "Could not open tests.json" << std::endl;
		return false;
	}

	json test_cases;
	file >> test_cases;
	int i = 1;
	// Iterate over each test case in the array
	for (const auto& test : test_cases)
	{
		// keep output of the wrong calculates
		std::ostringstream buffer;
		std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

		std::cout << "==========================================================================\n";
		std::cout << "Running test: " << test["name"] << "\n";
		currentName = test["name"];
		// Initial CPU values
		auto intVal = test["initial"];
		//emulator::CPU
		emulator::CPU.setRegJson('a', intVal.contains("a") ? (intVal["a"].get<int>()) : 0);
		emulator::CPU.setRegJson('b', intVal.contains("b") ? (intVal["b"].get<int>()) : 0);
		emulator::CPU.setRegJson('c', intVal.contains("c") ? (intVal["c"].get<int>()) : 0);
		emulator::CPU.setRegJson('d', intVal.contains("d") ? (intVal["d"].get<int>()) : 0);
		emulator::CPU.setRegJson('e', intVal.contains("e") ? (intVal["e"].get<int>()) : 0);
		emulator::CPU.setRegJson('f', intVal.contains("f") ? (intVal["f"].get<int>()) : 0);
		emulator::CPU.setRegJson('h', intVal.contains("h") ? (intVal["h"].get<int>()) : 0);
		emulator::CPU.setRegJson('l', intVal.contains("l") ? (intVal["l"].get<int>()) : 0);
		emulator::CPU.setRegJson('p', intVal.contains("pc") ? (intVal["pc"].get<int>()) : 0);
		emulator::CPU.setRegJson('s', intVal.contains("sp") ? (intVal["sp"].get<int>()) : 0);

		std::cout << "Initial CPU:\n";
		printCPU();
		std::cout << "------------------------\n";

		// RAM
		std::cout << "RAM:\n";
		if (test["initial"].contains("ram")) {
			for (const auto& entry : test["initial"]["ram"]) {
				if (entry.size() == 2) {
					uint16_t addr = (entry[0].get<int>());
					uint8_t val = (entry[1].get<int>());
					memory[addr] = val;

					if (memory[addr])
						std::cout << "  [" << std::hex << addr << "] = 0x" << +memory[addr] << "\n";
				}
			}
		}

		// make cpu run
		emulator::CPU.test();

		// Final CPU values
		auto final_cpu = test["final"];
		int8 fa = final_cpu.contains("a") ? (final_cpu["a"].get<int>()) : 0;
		int8 fb = final_cpu.contains("b") ? (final_cpu["b"].get<int>()) : 0;
		int8 fc = final_cpu.contains("c") ? (final_cpu["c"].get<int>()) : 0;
		int8 fd = final_cpu.contains("d") ? (final_cpu["d"].get<int>()) : 0;
		int8 fe = final_cpu.contains("e") ? (final_cpu["e"].get<int>()) : 0;
		int8 ff = final_cpu.contains("f") ? (final_cpu["f"].get<int>()) : 0;
		int8 fh = final_cpu.contains("h") ? (final_cpu["h"].get<int>()) : 0;
		int8 fl = final_cpu.contains("l") ? (final_cpu["l"].get<int>()) : 0;
		int16 fpc = final_cpu.contains("pc") ? (final_cpu["pc"].get<int>()) : 0;
		int16 fsp = final_cpu.contains("sp") ? (final_cpu["sp"].get<int>()) : 0;

		bool rightAnswer = compareRegs(fa, fb, fc, fd, fe, ff, fh, fl, fpc, fsp);

		std::cout << "------------------------\n";
		// reset mem
		if (test["initial"].contains("ram"))
			for (const auto& entry : test["initial"]["ram"])
				if (entry.size() == 2) {
					int16 addr = (entry[0].get<int>());
					memory[addr] = 0;
				}

		// reset cout
		std::cout.rdbuf(old); // Restore cout
		if (!rightAnswer) {
			std::cout << buffer.str();
			exit(-1);
			if (i++ == 2)
				return false;
		}
	}
	return true;
}

void gbMM::printEnd(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp)
{

	std::cout << "  A=" << +fa << "  B=" << +fb << "  C=" << +fc << "\n";
	std::cout << "  D=" << +fd << "  E=" << +fe << "  F=" << +ff << "\n";
	std::cout << "  H=" << +fh << "  L=" << +fl << "  PC=" << fpc << "  SP=" << fsp << "\n";

}

bool gbMM::compareRegs(int8 fa, int8 fb, int8 fc, int8 fd, int8 fe, int8 ff, int8 fh, int8 fl, int16 fpc, int16 fsp)
{
	bool ret = true;
	int8 a = emulator::CPU.getRegJson('a');
	int8 f = emulator::CPU.getRegJson('f');
	int8 b = emulator::CPU.getRegJson('b');
	int8 c = emulator::CPU.getRegJson('c');
	int8 d = emulator::CPU.getRegJson('d');
	int8 e = emulator::CPU.getRegJson('e');
	int8 h = emulator::CPU.getRegJson('h');
	int8 l = emulator::CPU.getRegJson('l');
	int16 pc = emulator::CPU.getRegJson('p');
	int16 sp = emulator::CPU.getRegJson('s');

	auto cmp = [](auto name, auto actual, auto expected) ->bool {
		bool res = actual == expected;
		std::string result = (actual == expected) ? "[V]" : "[X]";
		std::cout << result << " " << name << ": CPU = 0x"
			<< std::hex << +actual << ", expected = 0x" << +expected << "\n";
		return res;
		};

	std::cout << "\n======= REGISTER COMPARISON =======\n";
	ret = cmp("A", a, fa) && ret;
	ret = cmp("F", f, ff) && ret;
	ret = cmp("B", b, fb) && ret;
	ret = cmp("C", c, fc) && ret;
	ret = cmp("D", d, fd) && ret;
	ret = cmp("E", e, fe) && ret;
	ret = cmp("H", h, fh) && ret;
	ret = cmp("L", l, fl) && ret;
	ret = cmp("PC", pc, fpc) && ret;
	ret = cmp("SP", sp, fsp) && ret;
	std::cout << "===================================\n";
	return ret;
}

void gbMM::printCPU()
{
	int8 a = emulator::CPU.getRegJson('a');
	int8 b = emulator::CPU.getRegJson('b');
	int8 c = emulator::CPU.getRegJson('c');
	int8 d = emulator::CPU.getRegJson('d');
	int8 e = emulator::CPU.getRegJson('e');
	int8 f = emulator::CPU.getRegJson('f');
	int8 h = emulator::CPU.getRegJson('h');
	int8 l = emulator::CPU.getRegJson('l');
	int16 pc = emulator::CPU.getRegJson('p');
	int16 sp = emulator::CPU.getRegJson('s');
	auto cmp = [](auto name, auto actual) {
		std::cout << "[-]" << " " << name << ": CPU = 0x"
			<< std::hex << +actual << "\n";
		};

	std::cout << "\n======= CPU REGISTER =======\n";
	cmp("A", a);
	cmp("F", f);
	cmp("B", b);
	cmp("C", c);
	cmp("D", d);
	cmp("E", e);
	cmp("H", h);
	cmp("L", l);
	cmp("PC", pc);
	cmp("SP", sp);
	std::cout << "===================================\n";
}
