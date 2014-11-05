/* 
 * File:   PidFile.h
 * Author: Administrator
 *
 * Created on 2013��11��25��, ����10:27
 */

#ifndef PIDFILE_H
#define	PIDFILE_H
#include <stdint.h>
#include <string>
#include <vector>

namespace yeguang
{

class PidFile
{
    PidFile();
    PidFile(const PidFile& orig);
    virtual ~PidFile();
public:
    static PidFile *instance();
    
    int32_t CreatePidFile(std::string pathname);
    
    int32_t DeletePidFile(std::string pathname);
    
    bool IsExistForPidFile(std::string pathname);

    int32_t InsertPidToFile(std::string pathname, std::vector<int> pids);
    
    int32_t GetPidFromFile(std::string pathname, std::vector<int> &pids);
    
private:
    
};

}

#endif	/* PIDFILE_H */

