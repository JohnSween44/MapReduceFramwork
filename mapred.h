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

void threadMap(std::vector<std::string> vectorIn, int maps);


#endif
