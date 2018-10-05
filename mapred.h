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

void threadMap(std::vector< std::vector <std::string > > vectorIn, std::vector <std::pair<std::string, int> > *saveTo, int maps);

std::vector <std::vector <std::string> > vectorizer(std::ifstream *in, std::vector < std::string > *vectorIn ,int numMaps, int numReducers);

void initcombine(int type, std::vector< std::pair<std::string, int> > * listIn);

void reduce(int type, int numred, std::vector< std::pair<std::string, int> > * mapon);


#endif
