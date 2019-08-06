#pragma once

#include <string>

class Chip8
{
public:
	Chip8();

	void reset();

	void emulateCycle();

	bool loadROM(std::string path);

	unsigned char* getScreenArray();

	static const int WIDTH = 64;
	static const int HEIGHT = 32;

	static const int MEMORY_SIZE = 4096;
	
	//Is the engine required to play a beep sound this cycle
	bool beepThisCycle();
	
	//Should the engine refresh the screen this cycle
	bool isDrawFlagSet();

private:
	unsigned short opcode;

	unsigned char memory[MEMORY_SIZE];

	//Registers
	unsigned char V[16];

	unsigned short I; //Index Register
	unsigned short pc; //Program Counter

	unsigned char gameScreen[WIDTH * HEIGHT];

	unsigned char delayTimer;
	unsigned char soundTimer;

	unsigned short stack[16];
	unsigned short sp; // Stack Pointer

	unsigned char key[16];

	bool drawFlag;

	std::string convertOpcodeToPrintableHex(unsigned short op);
};