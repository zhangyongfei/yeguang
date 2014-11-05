#include "ConfigParse.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <stdlib.h>

namespace yeguang{

ConfigParse::ConfigParse(){

}

ConfigParse::~ConfigParse(){

}

int ConfigParse::Parse(std::string config_path){
    // ½âÎöÎÄ¼þ
    std::ifstream fin(config_path.c_str());

    if(!fin){
        std::cerr << config_path << " isn't existed." << std::endl;
        exit(1);
    }

    std::string line = "";
    int line_num = 0;

    while (fin >> line)
    {
        if(line.length() >= 2){
            std::string config = "";

            int index = line.find_first_of('#');

            if(index != std::string::npos){
                if(index >= 2){
                    config = line.substr(0, index);
                } else {
                    line_num++;
                    continue;
                }
            } else {
                config = line;
            }

            index = config.find_first_of('=');

            if(index != std::string::npos){
                if(index == 0){
                    std::cerr << "line " << line_num << " error." << std::endl;
                    exit(1);
                } else {
                    std::string name = config.substr(0, index);
                    std::string value = config.substr(index + 1);

                    ConfigMap::iterator iter = config_table_.find(name);

                    if(iter == config_table_.end()){
                        config_table_.insert(ConfigMap::value_type(name, TCfgInfo(value, line_num)));
                    } else {
                        iter->second = TCfgInfo(value, line_num);
                    }
                }
            } else {
                if(config.find_first_not_of(' ') >= 0){
                    std::cerr << "line " << line_num << " error." << std::endl;
                    exit(1);
                }
            }
        } else {
            if((line[0] != '#') || ((line.length() == 2)
                                      && line[0] != ' '
                                      && (line[1] != ' ' || (line[1] != '#')))){
                line_num++;
                continue;
            } else {
                std::cerr << "line " << line_num << " error." << std::endl;
                exit(1);
            }
        }
        line_num++;
    }
    return 0;
}

int ConfigParse::GetCofing(std::string name, std::string& value, std::string& line_num){
    ConfigMap::iterator iter = config_table_.find(name);

    if(iter == config_table_.end()){
        return -1;
    }

    value = iter->second.value_;
    line_num = iter->second.line_num_;
    return 0;
}

}
