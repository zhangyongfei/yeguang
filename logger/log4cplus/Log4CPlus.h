#ifndef LOGGER_MODULE
#define LOGGER_MODULE
#include <string>

#ifdef WIN32
#define LOG4CPLUS_BUILD_DLL
#endif

#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

/**
* @brief 重新定义日志级别
*
* @remarks
* @author chrono
* @version 1.0
* @since 2009/10/19   13:52
*/
enum StdLogLevel
{
	TraceLevel,
	DebugLevel,
	InfoLevel,
	WarningLevel,
	ErrorLevel,
	FatalLevel,
	OffLevel,
	AllLevel = TraceLevel
};

/**
* @brief 定义日志的输出
*
* @remarks
* @author chrono
* @version 1.0
* @since 2009/10/19   16:21
*/
enum StdLogAppend
{
	Console,
	RollingFile,
	DailyRollingFile
};

class CLoggerModule
{
public:
	// 单件 [2009/10/20 chrono]
	static CLoggerModule& instance();

	// 设置日志配置文件路径
	void ConfigLogger(std::string pathname = "log4cplus.properties");

	// 辅助格式函数 [2009/10/19 chrono]
	std::string bin2str(const unsigned char *buf, int nstrlen);
	const char* format(const char* szFormat, ...);

	// 流操作符输出 [2009/10/20 chrono]
	CLoggerModule& operator<<(StdLogLevel level);
	CLoggerModule& operator<<(const char *msg);
	CLoggerModule& operator<<(std::string &msg);

	// 设置日志记录器自身级别 [2009/10/20 chrono]
	CLoggerModule& operator=(StdLogLevel level);

private:
	StdLogLevel  m_infoLevel;
	StdLogLevel  m_logLevel;
	StdLogAppend m_append;

	void setLogLevel(StdLogLevel level);

	CLoggerModule(void);
	CLoggerModule(const CLoggerModule&);//noncopyable
	CLoggerModule& operator=(const CLoggerModule&);

	virtual ~CLoggerModule();
};

#define INITLOG4(pathname) CLoggerModule::instance().ConfigLogger(pathname)

#ifdef TRACE
#undef TRACE
#endif
#define LOG4TRACE(szFormat, ...)  LOG4CPLUS_TRACE(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef DEBUG
#undef DEBUG
#endif
#define LOG4DEBUG(szFormat, ...)  LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef INFO
#undef INFO
#endif
#define LOG4INFO(szFormat, ...)  LOG4CPLUS_INFO(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef WARN
#undef WARN
#endif
#define LOG4WARN(szFormat, ...)  LOG4CPLUS_WARN(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef ERROR
#undef ERROR
#endif
#define LOG4ERROR(szFormat, ...)  LOG4CPLUS_ERROR(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef FATAL
#undef FATAL
#endif
#define LOG4FATAL(szFormat, ...)  LOG4CPLUS_FATAL(log4cplus::Logger::getInstance("application"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#ifdef TITLELOG
#undef TITLELOG
#endif
#define LOG4TITLELOG(szFormat, ...)  LOG4CPLUS_INFO(log4cplus::Logger::getInstance("title"), \
	                           CLoggerModule::instance().format(szFormat, ##__VA_ARGS__))

#endif