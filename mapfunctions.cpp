#include "mapred.h"
#include <pthread.h>


void * threadWorker(void *);




void * threadWorker(void *arg){
threadInfo * thisThread = (threadInfo*)arg;

pthread_mutex_t *  mtx = thisThread->mutex;

std::vector <std::string > tempRead = *thisThread->readFrom; 


for(int i = 0; i < tempRead.size(); i++){
        //std::cout << tempRead[i] <<  "\n";
        pthread_mutex_lock(mtx);
        glb_vec.push_back(make_pair(tempRead[i], 1));
        pthread_mutex_unlock(mtx);
}
        
        pthread_exit(NULL);
}
        




