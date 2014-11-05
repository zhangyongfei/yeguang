#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <string>
#include <iomanip>

namespace yeguang
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		virtual void LoggerFatal(std::string content);
		virtual void LoggerError(std::string content);
		virtual void LoggerWarn(std::string content);
		virtual void LoggerInfo(std::string content);
		virtual void LoggerDebug(std::string content);
		virtual void LoggerTrace(std::string content);

	private:
	};
}

#endif