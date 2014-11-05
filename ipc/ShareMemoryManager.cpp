#include "ShareMemoryManager.h"
#include <string.h>

namespace yeguang{
    ShareMemoryManager::ShareMemoryManager(){
        master_ = NULL;
        child_ = NULL;
	}

	ShareMemoryManager::~ShareMemoryManager(){

	}

	int ShareMemoryManager::Init(int masterkey, int childkey, int pdusize, int pducnt){
        
        if(master_ != NULL){
        	master_->Exit();
        	delete master_;
        }

        master_ = new ShareMemoryQueue(masterkey);

        master_->Init(pdusize, pducnt);

        if(child_ != NULL){
        	child_->Exit();
        	delete child_;
        }

        child_ = new ShareMemoryQueue(childkey);

        child_->Init(pdusize, pducnt);

	    return 0;
	}

	int ShareMemoryManager::Exit(){

        if(master_ != NULL){
        	master_->Exit();
        	delete master_;
        }

        if(child_ != NULL){
        	child_->Exit();
        	delete child_;
        }

	    return 0;
	}

	int ShareMemoryManager::ToMaster(const char * const data, int datalen){        
        if(child_ == NULL){
        	return -1;
        }
         
		return child_->Write(data, datalen);
	}

	int ShareMemoryManager::FromMaster(char * const data, int datalen){
        if(master_ == NULL){
        	return -1;
        }

		return master_->Read(data, datalen);	
	}

	int ShareMemoryManager::ToChild(const char * const data, int datalen){
        if(master_ == NULL){
        	return -1;
        }

		return master_->Write(data, datalen);
	}

	int ShareMemoryManager::FromChild(char *const data, int datalen){
        if(child_ == NULL){
        	return -1;
        }
         
		return child_->Read(data, datalen);
	}
}
