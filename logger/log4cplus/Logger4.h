#ifndef _LOGGER4_
#define _LOGGER4_
#include "Logger.h"

namespace yeguang{
	class Logger4 : public Logger
	{
	public:
		Logger4(std::string config);
		~Logger4();

		void LoggerFatal(std::string content);
		void LoggerError(std::string content);
	    void LoggerWarn(std::string content);
		void LoggerInfo(std::string content);
		void LoggerDebug(std::string content);
		void LoggerTrace(std::string content);

		/* data */

	};
}



#endif