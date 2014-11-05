#include "Logger4.h"
#include "Log4CPlus.h"

namespace yeguang{

Logger4::Logger4(std::string config){
    INITLOG4(config);
}

Logger4::~Logger4(){
}

void Logger4::LoggerFatal(std::string content){
    LOG4FATAL(content.c_str());
}

void Logger4::LoggerError(std::string content){
	LOG4ERROR(content.c_str());
}
    
void Logger4::LoggerWarn(std::string content){
	LOG4WARN(content.c_str());
}

void Logger4::LoggerInfo(std::string content){
	LOG4INFO(content.c_str());
}

void Logger4::LoggerDebug(std::string content){
	LOG4DEBUG(content.c_str());
}

void Logger4::LoggerTrace(std::string content){
	LOG4TRACE(content.c_str());
}

}