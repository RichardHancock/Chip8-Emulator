#include "misc/Platform.h"
#include "misc/Log.h"
#include "Chip8.h"
#include "input/InputManager.h"

#include <thread>
#include <chrono>

int main(int argc, char* argv[]);

void render();

bool eventHandler();


Platform platform;
SDL_Renderer* renderer;
SDL_Texture* screenTex;

Chip8 c8;

const unsigned int screenArraySize = (Chip8::WIDTH * Chip8::HEIGHT) * (4 * sizeof(unsigned char));
unsigned char screenArray[screenArraySize];

int main(int argc, char* argv[])
{
	Log::init(false, "Richard Hancock", "Chip8 Emulator");

	if (argc < 2)
	{
		Log::logE("No ROM path passed through comand line parameters");
		return -1;
	}

	if (!platform.initSDL())
	{
		Log::logE("SDL Failed to initialize");
		exit(1);
	}

	renderer = platform.getRenderer();
	SDL_RenderSetLogicalSize(renderer, 64, 32);

	screenTex = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 64, 32);

	if (screenTex == NULL)
	{
		Log::logE(SDL_GetError());
	}

	//Load Program
	c8.loadROM(argv[1]);

	bool run = true;

	while (run)
	{
		//Emulate Cycle
		c8.emulateCycle();

		//Update Graphics if flag set
		if (c8.isDrawFlagSet())
		{
			//Should switch to lock/unlock texture
			render();
		}

		

		//Input
		run = eventHandler();



		InputManager::update();

		//Not Perfect But need to try to keep cycle running 60 times a second
		std::this_thread::sleep_for(std::chrono::microseconds(16600));
	}

	InputManager::cleanup();

	SDL_DestroyTexture(screenTex);

	return 0;
}

void render()
{
	for (int i = 0; i < Chip8::WIDTH * Chip8::HEIGHT; i++)
	{
		unsigned char pixel = c8.getScreenArray()[i] * 255;
		unsigned int offsetI = i * 4;

		screenArray[offsetI] = pixel;
		screenArray[offsetI + 1] = pixel;
		screenArray[offsetI + 2] = pixel;
		screenArray[offsetI + 3] = 0; //Alpha Chennel
	}

	SDL_UpdateTexture(screenTex, NULL, screenArray, Chip8::WIDTH * (4 * sizeof(unsigned char)));

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, screenTex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

bool eventHandler()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:
			InputManager::processKeyEvent(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			InputManager::processMouseEvent(e);

			break;

		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
			InputManager::processGameControllerEvent(e);
			break;
		}
	}

	if (InputManager::wasKeyReleased(SDLK_ESCAPE))
	{
		return false;
	}

	return true;
}