#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <math.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#ifndef MAPRED_H
#define MAPRED_H

void * threadWorker(void *arg);
void * threadWorkerRed(void *arg);
void finalReduce(std::vector< std::pair <std::string, int > > * sortMe);
void wordCombiner(std::vector< std::pair<std::string, int> > * keyValue);

typedef struct _threadInfo {
	std::vector <std::string >  * readFrom;
	std::vector <std::pair<std::string, int> >  * rd;
	std::vector <std::pair<std::string, int > > * wr;
	pthread_mutex_t * mutex; 	
}threadInfo;


extern std::vector<std::pair <std::string, int > > glb_vec;


#endif
