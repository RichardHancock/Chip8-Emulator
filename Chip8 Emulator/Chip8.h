#pragma once

class Chip8
{
public:
	Chip8();

	void reset();

	void emulateCycle();

	static const int WIDTH = 64;
	static const int HEIGHT = 32;

	static const int MEMORY_SIZE = 4096;


	bool isDrawFlagSet();

private:
	unsigned short opcode;

	unsigned char memory[MEMORY_SIZE];

	//Registers
	unsigned char V[16];

	unsigned short I; //Index Register
	unsigned short pc; //Program Counter

	unsigned char screen[WIDTH * HEIGHT];

	unsigned char delayTimer;
	unsigned char soundTimer;

	unsigned short stack[16];
	unsigned short sp; // Stack Pointer

	unsigned char key[16];

	bool drawFlag;
};