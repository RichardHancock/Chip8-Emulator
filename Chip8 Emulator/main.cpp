#include "misc/Platform.h"
#include "misc/Log.h"
#include "Chip8.h"
#include "input/InputManager.h"

#include <thread>
#include <chrono>

int main(int argc, char* argv[]);

void render();

bool eventHandler();

void passThroughInput();

Platform platform;
SDL_Renderer* renderer;
SDL_Texture* screenTex;

Chip8 c8;

const unsigned int screenArraySize = (Chip8::WIDTH * Chip8::HEIGHT) * (4 * sizeof(unsigned char));
unsigned char screenArray[screenArraySize];

// Keyboard layout
// 1 2 3 4 
// Q W E R
// A S D F
// Z X C V
int keyboardLayout[16] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v
};

int main(int argc, char* argv[])
{
	Log::init(false, "Richard Hancock", "Chip8 Emulator");

	if (argc < 2)
	{
		Log::logE("No ROM path passed through comand line parameters");
		return -1;
	}

	//Init Random
	srand((unsigned int)time(0));

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
		//Input
		run = eventHandler();

		passThroughInput();

		InputManager::update();

		//Emulate Cycle
		c8.emulateCycle();

		//Update Graphics if flag set
		if (c8.isDrawFlagSet())
		{
			//Should switch to lock/unlock texture
			render();
		}

		//Audio
		if (c8.beepThisCycle())
		{
			//Temp until I implement a audio solution
			Log::logD("BEEP");
		}

		//Not Perfect But need to try to keep cycle running 60 times a second
		//std::this_thread::sleep_for(std::chrono::microseconds(16600));
		std::this_thread::sleep_for(std::chrono::microseconds(8000));
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

void passThroughInput()
{
	for (int i = 0; i < 16; i++)
	{
		c8.setKeyState(i, InputManager::isKeyHeld(keyboardLayout[i]));
	}
}
