#ifndef CONFIG_PARSE
#define CONFIG_PARSE
#include <map>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

namespace yeguang{

class ConfigParse{
    typedef struct tagCfgInfo{
        std::string value_;
        std::string line_num_;

        tagCfgInfo(std::string value, int line_num){
            value_ = value;
            char tmp[100] = {0};
#ifdef WIN32
            sprintf_s(tmp, sizeof(tmp) - 1, "%d", line_num);
#else
            snprintf(tmp, sizeof(tmp) - 1, "%d", line_num);
#endif
            line_num_ = tmp;
        }
    }TCfgInfo;
    typedef std::map<std::string, TCfgInfo> ConfigMap;
public:
    ConfigParse();
    ~ConfigParse();

    int Parse(std::string config_path);

    int GetCofing(std::string name, std::string& value, std::string& line_num);

private:
    ConfigMap config_table_;
};

#define ConfgAssert(expression, error_content, line_num){ \
    if(!(expression)){ \
        if((line_num) != "") { \
            std::cerr << error_content << "line " << line_num << " error." << std::endl; \
            exit(1); \
        } else { \
            std::cerr << error_content << std::endl; \
            exit(1); \
        } \
    } \
}
}

#endif
