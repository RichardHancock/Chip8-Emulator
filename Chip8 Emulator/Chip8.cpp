#include "Chip8.h"

#include <cstdio>
#include "misc/Log.h"



unsigned char chip8FontSet[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8()
{
	reset();
}

void Chip8::reset()
{
	pc = 0x200; //Start at the Game in memory
	opcode = 0;
	I = 0;
	sp = 0;

	//Clear Screen
	for (int i = 0; i < WIDTH * HEIGHT; i++)
	{
		gameScreen[i] = 0;
	}

	//Clear Memory
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	//Clear Stack/Keys/Registers
	for (int i = 0; i < 16; i++)
	{
		V[i] = key[i] = 0;
		// Needs to be seperate as diff type, don't really want to add casts for a one time op.
		stack[i] = 0; 
	}

	//Reset Timers
	delayTimer = soundTimer = 0;


	//Load Fontset
	for (int i = 0; i < 80; i++)
	{
		memory[i] = chip8FontSet[i];
	}
}

void Chip8::emulateCycle()
{
	// Fetch Opcode (Opcodes are 2 bytes so merge both)
	opcode = memory[pc] << 8 | memory[pc + 1];

	//Testing the render process
	drawFlag = true;

	gameScreen[1] = 1;
	gameScreen[454] = 1;
	gameScreen[652] = 1;
	gameScreen[653] = 1;
	gameScreen[654] = 1;
	gameScreen[655] = 1;
	gameScreen[656] = 1;


	// Update timers
	if (delayTimer > 0)
		delayTimer--;

	if (soundTimer > 0)
		soundTimer--;
}

bool Chip8::loadROM(std::string path)
{
	FILE* programRaw = fopen(path.c_str(), "rb");

	if (programRaw == nullptr)
	{
		fclose(programRaw);

		Log::logE("Could not load requested ROM, likely a invalid path. Provided Path: " + path);
		return false;
	}

	Log::logI("Loading ROM: " + path);

	//Modified From: http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/comment-page-1/#comments

	// Check file size
	fseek(programRaw, 0, SEEK_END);
	long lSize = ftell(programRaw);
	rewind(programRaw);
	Log::logI("ROM Filesize: " + std::to_string(lSize));

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		Log::logE("Could not allocate memory to load ROM");
		fclose(programRaw);
		free(buffer);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, lSize, programRaw);
	if (result != (unsigned long)lSize)
	{
		Log::logE("Could not read ROM into buffer");
		fclose(programRaw);
		free(buffer);
		return false;
	}

	// Copy buffer to Chip8 memory
	if ((MEMORY_SIZE - 512) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			memory[i + 512] = buffer[i];
	}
	else
	{
		Log::logE("ROM too big for memory");
		fclose(programRaw);
		free(buffer);
		return false;
	}

	// Close file, free buffer
	fclose(programRaw);
	free(buffer);
	
	//END of copy

	return true;
}

unsigned char* Chip8::getScreenArray()
{
	return gameScreen;
}

bool Chip8::isDrawFlagSet()
{
	return drawFlag;
}
