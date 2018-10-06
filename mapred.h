#include <iostream>
#include <string>
#include <fstream>
#include "shared_mutex.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <sys/stat.h>
#include <string.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifndef MAPRED_H
#define MAPRED_H

using namespace std;


typedef struct VAL_PAIR{
        char word[5000000];
        int wordcount;
        int offset;
}memTracker;


void * threadWorker(void *arg);
void * threadWorkerRed(void *arg);
void numCounter(vector< pair<string, int> >, shared_mutex_t, memTracker*);
void mapper(char **argv);
void wordCounter(vector<string>, shared_mutex_t, memTracker*);
void numCounter(vector<string>, shared_mutex_t, memTracker*);
void reducer(char** argv);
bool comparePairsInts(std::pair<std::string, int>, std::pair<std::string, int>);
bool comparePairs(std::pair<std::string, int>, std::pair<std::string, int>);
void wordCombiner(vector< pair<string, int> >, shared_mutex_t , memTracker*);
void wordCombiner(vector< pair<string, int> > * keyValue);


typedef struct _threadInfo {
	std::vector <std::string >  * readFrom;
	std::vector <std::pair<std::string, int> >  * rd;
	std::vector <std::pair<std::string, int > > * wr;
	pthread_mutex_t * mutex; 	
}threadInfo;





extern std::vector<std::pair <std::string, int > > glb_vec;


#endif
