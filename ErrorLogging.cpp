#include "ErrorLogging.h"
#include <assert.h>			// for assert()
#include <iomanip>			// for put_time()
#include <fstream>			// for ofstream

static constexpr auto LOG_FILENAME = "log.txt";
static constexpr auto LOG_DATE_TIME_FORMAT = "[%d %B %Y - %T] "; // e.g. [09 June 2020 - 14:38:01]

static void AddErrorToLogFile(std::string errorDetails)
{
	// Get current calendar time and format it
	const auto now = time(0);
	#pragma warning(suppress : 4996); // supress localtime deprecation warning only for next line
	auto formattedTime = std::put_time(localtime(&now), LOG_DATE_TIME_FORMAT);

	// Open file in append mode (created if it doesnt exist)
	std::ofstream logFile(LOG_FILENAME, std::ios_base::app);
	
	// Add date, time and error details to log
	logFile << formattedTime << errorDetails << "\n";
	logFile.close();
}

void Assert(std::string errorDetails, bool condition)
{
	if (!condition)
	{
		AddErrorToLogFile(errorDetails);
		assert(condition);
	}
}

void DeleteLogFile()
{
	std::remove(LOG_FILENAME);
}