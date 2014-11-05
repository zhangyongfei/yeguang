#ifndef SHARE_MEMORY_QUEUE
#define SHARE_MEMORY_QUEUE

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

namespace yeguang{
    typedef struct tagQueueInfo{
        int datacnt_;
        int writepos_;
        int readpos_;
        int datasize_;
        char data[0];
    }TQueueInfo;

    class ShareMemoryQueue{
    public:
        ShareMemoryQueue(key_t shmkey);
        ~ShareMemoryQueue();

        int Init(int size, int pducnt);
        int Exit();

        int Write(const char * const data, int datalen);
        int Read(char * const data, int datalen);

    protected:
        TQueueInfo* getQueueInfo();



    private:    
        key_t shmkey_;

        int shmid_;
        void* shm_;

        int emptyid_;
        int fullid_;

        int maxpdusize_;

        
        struct sembuf P,V;
    };
}

#endif