/* 
 * File:   PidFile.cpp
 * Author: Administrator
 * 
 * Created on 2013年11月25日, 下午10:27
 */

#include "PidFile.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace yeguang
{

    PidFile::PidFile()
    {
    }

    PidFile::PidFile(const PidFile& orig)
    {
    }

    PidFile::~PidFile()
    {
    }

    PidFile *PidFile::instance()
    {
        static PidFile intanceobj;

        return &intanceobj;
    }

    static int LockFile(int fd, int cmd, int type, off_t offset, int whence, off_t len)
    {
        flock lock;

        lock.l_type = type; // F_RDLCK, F_WRLCK, F_UNLCK  
        lock.l_pid = getpid();
        lock.l_start = offset; // type offset, relative to l_whence  
        lock.l_whence = whence; // SEEK_SET, SEEK_CUR, SEEK_END  
        lock.l_len = 0;

        return (fcntl(fd, cmd, &lock));
    }

    bool PidFile::IsExistForPidFile(std::string pathname)
    {
        int32_t fd;
        if ((fd = open(pathname.c_str(), O_WRONLY, 644)) < 0)
        {
            if (errno == ENOENT)
            {
                return false;
            }
            return true;
        }

        if (LockFile(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                close(fd);
                return true;
            }
        }

        close(fd);
        return false;
    }

    int32_t PidFile::CreatePidFile(std::string pathname)
    {
        struct stat statbuf;
        int32_t fd;

        //打开文件并对文件写加锁
        if ((fd = open(pathname.c_str(), O_RDWR | O_CREAT | O_TRUNC, 644)) < 0)
        {
            perror("创建PID文件失败");
            return -1;
        }

        if (LockFile(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                DeletePidFile(pathname);
                return -1;
            }
        }

        //写入PPID PID到文件中
        if (ftruncate(fd, 0) < 0)
        {
            DeletePidFile(pathname);
            return -1;
        }

        char buffer[256];
        memset(buffer, 0, sizeof (buffer));

#ifdef _LINUX
        snprintf(buffer, sizeof (buffer), "%d", getpid());
#else
        sprintf(buffer, "%d", getpid());
#endif

        if (write(fd, buffer, strlen(buffer)) != strlen(buffer))
        {
            DeletePidFile(pathname);
            return -1;
        }

        /* turn on set-group-ID and turn off group-execute */
        if (fstat(fd, &statbuf) < 0)
        {
            DeletePidFile(pathname);
            return -1;
        }

        if (fchmod(fd, (statbuf.st_mode & ~(S_IXGRP | S_IWUSR)) | S_ISGID | S_IRUSR) < 0)
        {
            DeletePidFile(pathname);
            return -1;
        }

        // 设置文件为自动关闭
        int val;
        if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        {
            DeletePidFile(pathname);
            return -1;
        }

        val |= FD_CLOEXEC;
        if (fcntl(fd, F_SETFD, val) < 0)
        {
            DeletePidFile(pathname);
            return -1;
        }
        return 0;
    }

    int32_t PidFile::InsertPidToFile(std::string pathname, std::vector<int> pids)
    {
        int32_t fd;

        if ((fd = open(pathname.c_str(), O_RDWR, 644)) < 0)
        {
            perror("添加子进程");
            return -1;
        }

        int i = 0;

        std::string strPids = "";

        for(i = 0; i < pids.size(); i++){
            char buffer[256];
            memset(buffer, 0, sizeof (buffer));

    #ifdef _LINUX
            snprintf(buffer, sizeof (buffer), "\t%d", pids[i]);
    #else
            sprintf(buffer, "\t%d", pids[i]);
    #endif
            strPids += std::string(buffer);
        }

        lseek(fd, 0, SEEK_END);

        if (write(fd, strPids.c_str(), strPids.length()) != strPids.length())
        {
            perror("写入进程ID");
            close(fd);
            return -1;
        }

        close(fd);
        sleep(1);
        return 0;
    }

    int32_t PidFile::GetPidFromFile(std::string pathname, std::vector<int>& pids)
    {
        pids.clear();
        int32_t fd;
        if ((fd = open(pathname.c_str(), O_RDONLY, 644)) < 0)
        {
            return -1;
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int len = read(fd, buffer, sizeof(buffer));

        if(len > 0){
            std::string value = buffer;

            int start = 0;
            int index = value.find_first_of("\t");
            std::string id = "";

            while(index > 0){
                id = value.substr(start, index);

                if(id != ""){
                    pids.push_back(atoi(id.c_str()));
                }

                value = value.substr(index + 1);
                index = value.find_first_of("\t");
            }

            if(value != ""){
                pids.push_back(atoi(value.c_str()));
            }
        }

        close(fd);
        return 0;
    }

    int32_t PidFile::DeletePidFile(std::string pathname)
    {
        if (pathname != "")
        {
            // 删除文件
            unlink(pathname.c_str());
            pathname = "";
        }

        return 0;
    }
}

