#include "Log.h"
#include "Logger.h"

namespace yeguang
{
	static Logger g_logger;

	Logger* Log::m_logger = &g_logger;
    LoggerLevel Log::m_level = ALLLEVEL;

	void Log::CreateLog(Logger *logger)
	{
		if(logger == NULL)
		{
			return;
		}

		m_logger = logger;
	}

	void Log::Fatal(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < FATALLEVEL)
		{
			return;
		}

		m_logger->LoggerFatal(content);
	}

	void Log::Error(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < ERRORLEVEL)
		{
			return;
		}

		m_logger->LoggerError(content);
	}

	void Log::Warn(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < WARNLEVEL)
		{
			return;
		}

		m_logger->LoggerWarn(content);
	}

	void Log::Info(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < INFOLEVEL)
		{
			return;
		}

		m_logger->LoggerInfo(content);
	}

	void Log::Debug(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < DEBUGLEVEL)
		{
			return;
		}

		m_logger->LoggerDebug(content);
	}

	void Log::Trace(std::string content)
	{
		if(m_logger == NULL)
		{
			return;
		}

		if(m_level < TRACELEVEL)
		{
			return;
		}

		m_logger->LoggerTrace(content);
	}

	void Log::SetLevel(LoggerLevel level)
	{
		if(m_logger == NULL)
		{
			return;
		}
		
		m_level = level;
	}
}
