#ifndef SHARE_MEMORY_MANAGER
#define SHARE_MEMORY_MANAGER
#include "ShareMemoryQueue.h"

namespace yeguang{
	class ShareMemoryManager{
	public:
	    ShareMemoryManager();
	    ~ShareMemoryManager();

	    int Init(int masterkey, int childkey, int pdusize, int pducnt);

	    int Exit();

	    int ToMaster(const char * const data, int datalen);

	    int FromMaster(char * const data, int datalen);

	    int ToChild(const char * const data, int datalen);

	    int FromChild(char *const data, int datalen);

	private:
	    ShareMemoryQueue *master_;
	    ShareMemoryQueue *child_;
	};
}



#endif
