#include "Logger.h"
#include <iostream>
#include <time.h>

namespace yeguang
{
#ifdef WIN32
#define LogOutput(level, content) do{\
	time_t timer; \
	struct tm tblock;\
	timer = time(NULL);\
	localtime_s(&tblock, &timer);\
	\
	std::cout <<"[" << std::setfill('0') << std::setw(4) << (1900 + tblock.tm_year) << "-" \
	     << std::setfill('0') << std::setw(2) << (tblock.tm_mon + 1) << "-" \
	     << std::setfill('0') << std::setw(2) << tblock.tm_mday << " "\
		<< std::setfill('0') << std::setw(2)  << tblock.tm_hour << ":" \
		<< std::setfill('0') << std::setw(2)  << tblock.tm_min << ":" \
		<< std::setfill('0') << std::setw(2)  << tblock.tm_sec \
		<< "][" << level << "]" << content << std::endl;\
}while(0);
#else
#define LogOutput(level, content) do{\
		time_t now; \
		struct tm *tblock; \
		time(&now); \
		tblock = localtime(&now); \
	\
	std::cout <<"[" << std::setfill('0') << std::setw(4) << (1900 + tblock->tm_year) << "-" \
	     << std::setfill('0') << std::setw(2) << (tblock->tm_mon + 1) << "-" \
	     << std::setfill('0') << std::setw(2) << tblock->tm_mday << " "\
		<< std::setfill('0') << std::setw(2)  << tblock->tm_hour << ":" \
		<< std::setfill('0') << std::setw(2)  << tblock->tm_min << ":" \
		<< std::setfill('0') << std::setw(2)  << tblock->tm_sec \
		<< "][" << level << "]" << content << std::endl;\
}while(0);
#endif

	Logger::Logger()
	{

	}

	Logger::~Logger()
	{

	}

	void Logger::LoggerFatal(std::string content)
	{
        LogOutput("FATAL", content);		
	}

	void Logger::LoggerError(std::string content)
	{
		LogOutput("ERROR", content);
	}

	void Logger::LoggerWarn(std::string content)
	{
		LogOutput("WARN ", content);
	}

	void Logger::LoggerInfo(std::string content)
	{
		LogOutput("INFO ", content);
	}

	void Logger::LoggerDebug(std::string content)
	{
        LogOutput("DEBUG", content);
	}

	void Logger::LoggerTrace(std::string content)
	{
		LogOutput("TRACE", content);
	}

}