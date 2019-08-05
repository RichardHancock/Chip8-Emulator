#pragma once

#include <SDL.h>
#include <string>
#include <fstream>

//Ref: Was used in previous assignment

/** 
@brief	Contains the Engine's logging features.
*/
class Log
{
public:
	/** 
	@brief A type of log message
	*/
	enum LogType
	{
		E, ///< Error
		W, ///< Warning
		I, ///< Info
		D  ///< Debug
	};

	static bool init(bool fileOutput, std::string org, std::string app);

	static void cleanup();

	/**
	@brief Log a message to the console

	Made this one function instead of multiple to reduce clutter. Uses SDL_Log internally to make it
	function on multiple platforms easily.

	@param type    Type of message E: Error, W: Warning, I: Info, D: Debug.
	@param message The message to log.
	*/
	static void log(LogType type, std::string message);

	/**
	@brief Log a debug message to the console. Only displayed for a debug build.

	@param message The message to log.
	*/
	static void logD(std::string message);

	/**
	@brief Log a info message to the console

	@param message The message to log.
	*/
	static void logI(std::string message);

	/**
	@brief Log a warning message to the console

	@param message The message to log.
	*/
	static void logW(std::string message);

	/**
	@brief Log a error message to the console

	@param message The message to log.
	*/
	static void logE(std::string message);


	static void LogOutputFunction(void* userdata, int category, SDL_LogPriority priority, const char* message);

private:
	
	static bool initialized;

	static std::string convertSDL_LogPriority(SDL_LogPriority priority);

	static std::ofstream logFile;
};