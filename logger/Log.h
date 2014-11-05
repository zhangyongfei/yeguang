#ifndef _LOG_H_
#define _LOG_H_
#include <string>
#include <sstream>
#include <iomanip>

namespace yeguang
{

typedef enum
{
	OFFLEVEL = 0,
	FATALLEVEL,
	ERRORLEVEL,
	WARNLEVEL,
	INFOLEVEL,
	DEBUGLEVEL,
	TRACELEVEL,
	ALLLEVEL,
}LoggerLevel;

class Logger;

class Log
{
public:
	static void CreateLog(Logger *logger);

	static void Fatal(std::string content);
	static void Error(std::string content);
	static void Warn(std::string content);
	static void Info(std::string content);
	static void Debug(std::string content);
	static void Trace(std::string content);
	static void SetLevel(LoggerLevel level);

private:
	static Logger *m_logger;
	static LoggerLevel m_level;
};

#define LogFatal(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Fatal(tmp.str()); \
} while (0)

#define LogError(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Error(tmp.str()); \
} while (0)

#define LogWarn(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Warn(tmp.str()); \
} while (0);

#define LogInfo(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Info(tmp.str()); \
} while (0)

#define LogDebug(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Debug(tmp.str()); \
} while (0)

#define LogTrace(content) do \
{ \
	std::ostringstream tmp; \
	tmp << content; \
	yeguang::Log::Trace(tmp.str()); \
} while (0)

}

#endif