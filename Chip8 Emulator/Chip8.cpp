#include "Chip8.h"

#include <cstdio>
#include <sstream>

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

	//Compare first 4 bits
	switch (opcode & 0xF000)
	{
	//0x0
	case 0x0000: //First 4 bits not enough so need to compare last 12 bits
		switch (opcode & 0x0FFF)
		{
		case 0x00E0: // 00E0 - Clear Screen

			break;
		case 0x00EE: // 00EE - Return from Subroutine

			break;

		default: //0x0NNN - Calls RCA 1802 program at address NNN. Not necessary for emulators according to a few sources.
			Log::logW("Unimplemented or Unknown opcode: " + convertOpcodeToPrintableHex(opcode));
			break;
		}
		break;

	//0x1
	case 0x1000: //1NNN - Jump to location NNN
		//pc = 
		break;

	//0x2
	case 0x2000: //2NNN - Call Subroutine at NNN

		break;

	//0x3
	case 0x3000: //3XNN - Skip Next Instruction if (Vx == NN)

		break;

	//0x4
	case 0x4000: //4XNN - Skip Next Instruction if (Vx != NN)

		break;

	//0x5
	case 0x5000: //5XY0 - Skip Next Instruction if (Vx == Vy)

		break;

	//0x6
	case 0x6000: //6XNN - Set Vx Register to NN

		break;

	//0x7
	case 0x7000: //7XNN - Adds NN to Vx Then Stores Result in Vx (Vx += NN)

		break;

	//0x8
	case 0x8000: //First 4 bits not enough so need to compare last 4 bits
		switch (opcode & 0x000F)
		{
		case 0x0000: //8XY0 - Set Vx Register to Vy (Vx = Vy)
			
			break;
		case 0x0001: //8XY1 - Bitwise OR (Vx = (Vx | Vy))

			break;
		case 0x0002: //8XY2 - Bitwise AND (Vx = (Vx & Vy))

			break;
		case 0x0003: //8XY3 - Bitwise XOR (Vx = (Vx ^ Vy))

			break;
		case 0x0004: //8XY4 - Add Vx to Vy and store in Vx (Vx += Vy). If result greater then 255 VF carry flag needs to be set

			break;
		case 0x0005: //8XY5 - Subtract Vy from Vx and store in Vx (Vx -= Vy). If Vx > Vy set VF to 1

			break;
		case 0x0006: //8XY6 - If the least significant bit of Vx is 1, then VF is set to 1. Then Vx is divided by 2.

			break;
		case 0x0007: //8XY7 - Subtract Vx from Vy and store in Vx (Vx = Vy - Vx). If Vy > Vx set VF to 1

			break;
		case 0x000E: //8XYE - If the most significant bit of Vx is 1, then VF is set to 1. Then Vx is multiplied by 2.

			break;

		default:
			Log::logW("Unknown opcode: " + convertOpcodeToPrintableHex(opcode));
			break;
		}
		break;

	//0x9
	case 0x9000: //9XY0 - Skip Next Instruction if (Vx != Vy)

		break;

	//0xA
	case 0xA000: //ANNN - Sets I to the memory address NNN.
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	//0xB
	case 0xB000: //BNNN - Jump to location NNN + V0

		break;

	//0xC
	case 0xC000: //CXNN - Generate Random Number Between 0-255, then AND with NN and store in Vx. (Vx = (rand(0-255) & NN))

		break;

	//0xD
	case 0xD000: //DXYN - Draw sprite of N bytes starting at I in memory at pos (Vx, Vy). Set VF for collision event.

		break;

	//0xE
	case 0xE000: //First 4 bits not enough so need to compare last 8 bits
		switch (opcode & 0x00FF)
		{
		case 0x009E: //EX9E - Skip Next Instruction if key[Vx] is Pressed

			break;
		case 0x00A1: //EXA1 - Skip Next Instruction if key[Vx] is not Pressed

			break;

		default:
			Log::logW("Unknown opcode: " + convertOpcodeToPrintableHex(opcode));
			break;
		}
		break;

	//0xF
	case 0xF000: //First 4 bits not enough so need to compare last 8 bits
		switch (opcode & 0x00FF)
		{
		case 0x0007: //FX07 - Set Vx to delay timer value
			
			break;
		case 0x000A: //FX0A - Wait for key press, value of key stored in Vx. This Blocks All Execution Until Key Press.

			break;
		case 0x0015: //FX15 - Set Delay Timer to Vx

			break;
		case 0x0018: //FX18 - Set Sound Timer to Vx

			break;
		case 0x001E: //FX1E - Add Vx to I and store result in I

			break;
		case 0x0029: //FX29 - Set I to the memory location of sprite for character stored in Vx

			break;
		case 0x0033: //FX33 - Store the Binary-Coded Decimal representation of Vx in memory at locations I, I+1, I+2

			break;
		case 0x0055: //FX55 - Dump values from registry (V0 - Vx) in memory at address I and onwards. 'I' should not be modified

			break;
		case 0x0065: //FX065 - Load values from registry (V0 - Vx) in memory at address I and onwards. 'I' should not be modified

			break;

		default:
			Log::logW("Unknown opcode: " + convertOpcodeToPrintableHex(opcode));
			break;
		}
		break;
	default:
		Log::logW("Unknown opcode: " + convertOpcodeToPrintableHex(opcode));
		break;
	}
	//pc += 2;
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
		//fclose(programRaw);

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
		//free(buffer);
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

bool Chip8::beepThisCycle()
{
	return soundTimer == 1;
}

bool Chip8::isDrawFlagSet()
{
	return drawFlag;
}

std::string Chip8::convertOpcodeToPrintableHex(unsigned short op)
{
	std::stringstream ss;
	ss << std::hex << std::showbase << op;
	return std::string(ss.str());
}
