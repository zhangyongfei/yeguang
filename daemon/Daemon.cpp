#include "Daemon.h"
#include "PidFile.h"
#include <stdio.h>
#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#include<sys/wait.h>
#include <signal.h>
#include <iostream>
#endif

namespace yeguang{

Daemon::Daemon(std::string pid_path, int child ){
    pid_path_ = pid_path;
    child_ = child;
}

Daemon::~Daemon(){

}

int Daemon::StartSignle(){
    if(PidFile::instance()->CreatePidFile(pid_path_) != 0){
        return -1;
    }
    return Worker();
}

int Daemon::StartDaemon(){
#ifdef _WIN32
    if(PidFile::instance()->CreatePidFile(pid_path_) != 0){
        return -1;
    }
	return Worker();
#else
    if(child_ <= 0){
        if(PidFile::instance()->CreatePidFile(pid_path_) != 0){
            return -1;
        }
        return Worker();
    }
    pid_t parentpid;
	parentpid = fork();

	if (parentpid < 0)        
		return -1;
	else if (parentpid == 0){

		pid_t pid;
		int status = -1;
		
        int i = 0;

        std::vector<int> pids;

        if(PidFile::instance()->CreatePidFile(pid_path_) != 0){
            return -1;
        }

        while(status != 0){
            while(i < child_)
            {
                pid = fork();
                if (pid < 0){
                    return -1;
                }
                else if (pid == 0){                    
                    int res = Worker();
                    return res;
                }
                pids.push_back(pid);
                i++;
            }

            if(PidFile::instance()->InsertPidToFile(pid_path_, pids) != 0){
                return -1;
            }

            Run();
            waitpid(-1, &status, 0);
		}

		return 0;
	}
	return 0;
#endif
}

int Daemon::Stop(){
    std::vector<int> pids;
    if(PidFile::instance()->GetPidFromFile(pid_path_, pids) == 0){
        PidFile::instance()->DeletePidFile(pid_path_);
    }

    int i = 0;

    for(i = 0; i < pids.size(); i++){
        if(pids[i] > 0){
            kill(pids[i], 9);
            sleep(1);
        }
    }

    return 0;
}

int Daemon::Run(){
    return 0;
}

}
