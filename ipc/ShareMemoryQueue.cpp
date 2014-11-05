#include "ShareMemoryQueue.h"
#include <string.h>
#include <stdio.h>

namespace yeguang{
union semun 
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}arg;

ShareMemoryQueue::ShareMemoryQueue(key_t shmkey){
    shmkey_ = shmkey;

    shmid_ = -1;
    shm_ = NULL;

    emptyid_ = -1;
    fullid_  = -1;

    P.sem_num=0;
    P.sem_op=-1;
    P.sem_flg=SEM_UNDO;

    V.sem_num=0;
    V.sem_op=1;
    V.sem_flg=SEM_UNDO;
}

ShareMemoryQueue::~ShareMemoryQueue(){

}

int ShareMemoryQueue::Init(int size, int pducnt){
    // 初始化信号量
    emptyid_ = semget(shmkey_ + 0x5f000000, 1, IPC_CREAT|0666);

    if(emptyid_ == -1){
        perror("empty semget error\n");
        return -1;
    }

    fullid_ = semget(shmkey_ + 0x6f000000, 1, IPC_CREAT|0666);

    if(fullid_ == -1){
        perror("full semget error\n");
        return -1;
    }

    union semun arg;

    arg.val = pducnt;   //初始时互斥信号为1,允许一个进程进入

    if(semctl(emptyid_, 0, SETVAL, arg)==-1){
        perror("semctl setval error");
        return -1;
    }  

    arg.val=0;                            //初始时缓冲区中无数据

    if(semctl(fullid_,0,SETVAL,arg)==-1){
        perror("semctl setval error");
        return -1;
    }       

    // 初始化共享内存
    maxpdusize_ = size;

    shmid_ = shmget(shmkey_, size * pducnt + sizeof(TQueueInfo), IPC_CREAT | 0640);

    if(shmid_ == -1){
        perror("shmget error\n");
        return -1;
    }

    shm_ = shmat(shmid_, NULL, 0);

    if (shm_ == NULL)
    {
        perror("shmat error\n");
        return -1;
    }

    TQueueInfo* info = (TQueueInfo*)getQueueInfo();

    if(info == NULL){
        return -1;
    }

    info->datacnt_  = 0;
    info->writepos_ = 0;
    info->readpos_  = 0;
    info->datasize_ = pducnt;

    return 0;
}

int ShareMemoryQueue::Exit(){
    if(shm_ == NULL){
        return -1;
    }

    // 删除共享内存
    if(shmdt(shm_) == -1){
        perror(" detach error ");
        return -1;
    }
       
    if (shmctl(shmid_ , IPC_RMID , NULL) == -1){
        perror(" delete error ");
        return -1;  
    }

    return 0;      
}

int ShareMemoryQueue::Write(const char * const data, int datalen){
    if (datalen > maxpdusize_)
    {
        return -1;
    }

    if(data == NULL){
        return maxpdusize_;
    }

    semop(emptyid_, &P, 1);

    TQueueInfo* info = (TQueueInfo*)getQueueInfo();

    if(info != NULL){

        if (info->datacnt_ < info->datasize_)
        {
            memset(&info->data[info->writepos_ * maxpdusize_], 0, maxpdusize_);
            memcpy(&info->data[info->writepos_ * maxpdusize_], data, datalen);

            info->writepos_++;
            info->datacnt_++;
            if (info->writepos_ >= info->datasize_)
            {
                info->writepos_ = 0;
            }
        }
    }

    semop(fullid_,&V,1);

    return 0;
}

int ShareMemoryQueue::Read(char * const data, int datalen){
    if(datalen < maxpdusize_){
        return -1;
    }

    if(data == NULL){
        return maxpdusize_;
    }

    memset(data, 0, datalen);

    semop(fullid_,&P,1);
    
    TQueueInfo* info = (TQueueInfo*)getQueueInfo();

    if(info != NULL){

        if (info->datacnt_ > 0)
        {
            memcpy(data, &info->data[info->readpos_ * maxpdusize_], maxpdusize_);

            info->readpos_++;
            info->datacnt_--;
            if (info->readpos_ >= info->datasize_)
            {
                info->readpos_ = 0;
            }
        }
    }
    
    semop(emptyid_,&V,1);

    return maxpdusize_;
}

TQueueInfo* ShareMemoryQueue::getQueueInfo(){
    if(shm_ == NULL){
        return NULL;
    }

    return (TQueueInfo*)shm_;
}   
}

