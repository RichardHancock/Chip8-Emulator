#include "Log.h"

#include <time.h>
#include <iostream>

bool Log::initialized = false;
std::ofstream Log::logFile;

bool Log::init(bool fileOutput, std::string org, std::string app)
{
	
#ifndef __ANDROID__ //SDL handles android output

	if (!initialized)
	{
		//typedef void(*SDL_LogOutputFunction)(void *userdata, int category, SDL_LogPriority priority, const char *message);
		SDL_LogOutputFunction callback = &LogOutputFunction;
		SDL_LogSetOutputFunction(callback, nullptr);

		if (fileOutput)
		{
			std::string filename;

			char* basePath = SDL_GetPrefPath(org.c_str(), app.c_str());
			std::string path = basePath;
			path += "/";
			SDL_free(basePath);

			//Get current time and date
			time_t currentTime = time(NULL);

			char textTime[20];

	#ifdef _MSC_VER
			//Because Microsoft like to be different they depreciated the normal localtime(), and most compilers implement localtime_r() not _s
			tm timeStruct;
			localtime_s(&timeStruct, &currentTime);

			strftime(textTime, 20, "%Y-%m-%d %H-%M-%S", &timeStruct);

	#else

			tm* timeStruct;
			timeStruct = localtime(&currentTime);

			strftime(textTime, 20, "%Y-%m-%d %H-%M-%S", timeStruct); //Apparently only Microsoft don't support the %T.

	#endif

			filename = textTime;
			filename += " Log.log";
			path += filename;

			logFile.open(path, std::ios::out | std::ios::app);

			if (!logFile.is_open())
				Log::logW("Log File Could not be opened, console output only.");
		}
	}
	else
	{
		Log::logW("Log SubSystem already initialized");
		return true;
	}

#endif // __ANDROID__

	Log::logI("Log SubSystem Initialised");
	initialized = true;
	return true;
}

void Log::cleanup()
{
	logFile.close();
}

void Log::log(LogType type, std::string message)
{
	//Get current time
	time_t currentTime = time(NULL);

	char textTime[9];

#ifdef _MSC_VER
	//Because Microsoft like to be different they depreciated the normal localtime(), and most compilers implement localtime_r() not _s
	tm timeStruct;
	localtime_s(&timeStruct, &currentTime);

	strftime(textTime, 9, "%H:%M:%S", &timeStruct);

#else

	tm* timeStruct;
	timeStruct = localtime(&currentTime);

	strftime(textTime, 9, "%T", timeStruct); //Apparently only Microsoft don't support the %T.

#endif

	std::string finalMsg = textTime;

	switch (type)
	{
	case D:
	#ifdef _DEBUG
		finalMsg += (" " + message);
		//Have to use category Test as Debug messages are not printed for Application.
		SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "%s", finalMsg.c_str());
	#endif // NDEBUG
		break;
	case I:
		finalMsg += (" " + message);
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", finalMsg.c_str());
		break;

	case W:
		finalMsg += (" " + message);
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", finalMsg.c_str());
		break;

	case E:
		finalMsg += (" " + message);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", finalMsg.c_str());
		break;

	default:
		finalMsg += (" Uncategorised: " + message);
		SDL_Log("%s", finalMsg.c_str());
		break;

	}
}

void Log::logD(std::string message)
{
	Log::log(Log::D, message);
}

void Log::logI(std::string message)
{
	Log::log(Log::I, message);
}

void Log::logW(std::string message)
{
	Log::log(Log::W, message);
}

void Log::logE(std::string message)
{
	Log::log(Log::E, message);
}

void Log::LogOutputFunction(void*, int, SDL_LogPriority priority, const char * message)
{
	std::cout << convertSDL_LogPriority(priority) << ": " << message << std::endl;

	if (logFile.is_open())
	{
		logFile << convertSDL_LogPriority(priority) << ": " << message << std::endl;
	}
}

std::string Log::convertSDL_LogPriority(SDL_LogPriority priority)
{
	switch (priority)
	{
	case SDL_LOG_PRIORITY_VERBOSE:
		return "Verbose";
		break;
	case SDL_LOG_PRIORITY_DEBUG:
		return "Debug";
		break;
	case SDL_LOG_PRIORITY_INFO:
		return "Info";
		break;
	case SDL_LOG_PRIORITY_WARN:
		return "Warning";
		break;
	case SDL_LOG_PRIORITY_ERROR:
		return "Error";
		break;
	case SDL_LOG_PRIORITY_CRITICAL:
		return "Critical";
		break;
	default:
		return "Unknown Priority";
		break;
	}
}
