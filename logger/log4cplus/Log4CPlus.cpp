#include "Log4CPlus.h"
#include <log4cplus/configurator.h>


#include <cstdio>
#include <cstdarg>

#include <vector>
#include <sstream>

using namespace std;
using namespace log4cplus;

// 日志的输出格式 [2009/10/19 chrono]
#define MYLOG_PATTERN "{%D} {%-5p} {%t}  - %m %n"
#define LOG_OBJ   Logger::getRoot()


/**   CLoggerModule
* fullname: CLoggerModule::CLoggerModule
* access:   private 
* @brief 
*    
* @param void 
* @return  函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/20  10:05
*/
CLoggerModule::CLoggerModule()
{
	m_infoLevel = InfoLevel;
	m_logLevel = TraceLevel;
}

CLoggerModule::~CLoggerModule(void)
{
}

/**   instance
* fullname: CLoggerModule::instance
* access:   public 
* @brief 
*    
* @return CLoggerModule& 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  9:45
*/
CLoggerModule& CLoggerModule::instance()
{
	static CLoggerModule alog;

	return alog;
}

void CLoggerModule::ConfigLogger(std::string pathname /*= "log4cplus.properties"*/)
{
	log4cplus::initialize();

	PropertyConfigurator::doConfigure(pathname);
}

/**   operator<<
* fullname: CLoggerModule::operator<<
* access:   public 
* @brief 
*    
* @param StdLogLevel * level 
* @return CLoggerModule& 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  13:40
*/
CLoggerModule& CLoggerModule::operator<<( StdLogLevel level )
{
	m_infoLevel = level;

	return *this;
}
/**   operator<<
* fullname: CLoggerModule::operator<<
* access:   public 
* @brief 重载《《，输出info日志
*    
* @param const char * msg 
* @return CLoggerModule& 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  13:20
*/
CLoggerModule& CLoggerModule::operator<<( const char *msg )
{
	switch(m_infoLevel)
	{
	case TraceLevel:
		LOG4TRACE(msg);break;
	case DebugLevel:
		LOG4DEBUG(msg);break;
	case InfoLevel:
		LOG4INFO(msg);break;
	case WarningLevel:
		LOG4WARN(msg);break;
	case ErrorLevel:
		LOG4ERROR(msg);break;
	case FatalLevel:
		LOG4FATAL(msg);break;
	default://off
		break;//info(msg);
	}

	return *this;
}

/**   format
* fullname: CLoggerModule::format
* access:   public 
* @brief 
*    
* @param const char * szFormat 
* @param ... 
* @return std::string 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  16:51
*/
const char* CLoggerModule::format( const char* szFormat, ... )
{
	char buffer[8192] = {0};

	va_list args;

	// retrieve the variable arguments
	va_start( args, szFormat );
#ifdef _WIN32
    vsprintf_s(&buffer[0], sizeof(buffer), szFormat, args ); // C4996
#else	
	snprintf(&buffer[0], sizeof(buffer), szFormat, args );
#endif

	va_end(args);

	return &buffer[0];
}

/**   bin2str
* fullname: CLoggerModule::bin2str
* access:   public 
* @brief 将缓冲数据转换为十六进制字符串
*    
* @param unsigned char * buf 
* @param int nstrlen 
* @return std::string 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  10:00
*/
std::string CLoggerModule::bin2str( const unsigned char *buf, int nstrlen )
{
	stringstream ss;

	ss.setf(ios::hex | ios::uppercase );
	ss.width(2);
	ss.fill('0');
	for (int i = 0;i < nstrlen ; ++i)
	{
		ss << hex << (short)buf[i];
	}

	return ss.str();

}

/**   setLogLevel
* fullname: CLoggerModule::setLogLevel
* access:   public 
* @brief 设置日志记录器的级别
*    
* @param StdLogLevel level 
* @return void 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  14:22
*/
void CLoggerModule::setLogLevel( StdLogLevel level )
{
	m_logLevel = level;

	switch(m_logLevel)
	{
	case TraceLevel:
		LOG_OBJ.setLogLevel(TRACE_LOG_LEVEL);break;
	case DebugLevel:
		LOG_OBJ.setLogLevel(DEBUG_LOG_LEVEL);break;
	case WarningLevel:
		LOG_OBJ.setLogLevel(WARN_LOG_LEVEL);break;
	case ErrorLevel:
		LOG_OBJ.setLogLevel(ERROR_LOG_LEVEL);break;
	case FatalLevel:
		LOG_OBJ.setLogLevel(FATAL_LOG_LEVEL);break;
	case OffLevel:
		LOG_OBJ.setLogLevel(OFF_LOG_LEVEL);break;
	default:
		LOG_OBJ.setLogLevel(INFO_LOG_LEVEL);break;
	}

}

/**   operator>>
* fullname: CLoggerModule::operator>>
* access:   public 
* @brief 同setLogLevel
*    
* @param StdLogLevel level 
* @return CLoggerModule& 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  14:38
*/
CLoggerModule& CLoggerModule::operator=( StdLogLevel level )
{
	setLogLevel(level);

	return *this;
}

/**   operator<<
* fullname: CLoggerModule::operator<<
* access:   public 
* @brief 
*    
* @param std::string & msg 
* @return CLoggerModule& 函数执行情况 
* @retval 0代表成功，非0代表失败
* @author chrono
* @since 2009/10/19  15:56
*/
CLoggerModule& CLoggerModule::operator<<( std::string &msg )
{
	return operator<<(msg.c_str());
}