#include "misc/Platform.h"
#include "misc/Log.h"
#include "Chip8.h"

#include <thread>
#include <chrono>

int main(int argc, char* argv[]);

int main(int, char*[])
{
	Log::init(false, "Richard Hancock", "Chip8 Emulator");

	Platform platform;

	if (!platform.initSDL())
	{
		Log::logE("SDL Failed to initialize");
		exit(1);
	}

	SDL_Renderer* renderer = platform.getRenderer();
	SDL_RenderSetLogicalSize(renderer, 64, 32);


	Chip8 c8;

	bool run = true;

	while (run)
	{
		//Emulate Cycle

		//Update Graphics if flag set

		//Input

		//Not Perfect But need to try to keep cycle running 60 times a second
		std::this_thread::sleep_for(std::chrono::microseconds(16600));
	}

	return 0;
}