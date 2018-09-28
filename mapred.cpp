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

void vectorizer(std::ifstream*, std::vector<std::string>*, int, int);

int main(int argc, char ** argv){

        //Variable initialization
        int app;
        int impl;
        int num_maps = atoi(argv[3]);
        int num_reduces = atoi(argv[4]);
        char* input = argv[5];
        char* output = argv[6];

        //Update variables from command line
        if(argv[1] == "wordcount"){
                app = 1;
        }else if(argv[1] == "sort"){
                app = 0;
        }else{
                app = -1;
        }

        if(argv[2] == "procs"){
                impl = 1;
        }else if(argv[2] == "threads"){
                impl = 0;
        }else{
                impl = -1;
        }

        //if(app == -1 || impl == -1){
        //      cout << "Invalid Input!";
        //}else{
        //
        //}
        std::ifstream readin;
        readin.open(input);

        //Vector of pairs to read into
        std::vector < std::string > totList;

        //Tokenize
        vectorizer(&readin, &totList, num_maps, num_reduces);

        /*
        for (int i = 0; i < totList.size(); i++){
                std::cout << totList[i] << "\n";
        }
        */
        std::cout << "Total Size: " << totList.size() << "\n";

        return 0;
}



/*
Working as of 9/21/18
Input:
        std::ifstream * in
                - File input stream, already opened.
        std::vector <std::string> * vectorIn
                - External vector that function will read words from file into
Output:
        -Tokenzied vector of all the words present in file passed in to function.
        -vector of vectors of tokenized input
*/
void vectorizer(std::ifstream *in, std::vector < std::string > *vectorIn, int numMaps, int numReducers){

        //Open file input stream and set local vars for input and vector;
        std::ifstream * mapRead = in;
        std::vector < std::string > * vec = vectorIn;

        //Variable to hold the word
        std::string a;

        //Goes through file while has input and adds to passed in array.
        while (*in >> a)
        {
                std::transform(a.begin(), a.end(), a.begin(), ::tolower);
                vec->push_back( a );
        }

        //std::sort(vec->begin(), vec->end());

        //the following block of code is to partion the vector of all text to semi-evenly
        //filled sub vectors, these sub vectors are then stored into a vector
        //first step: use ceiling function to find at most how many elements each vector hass
        int numElements = ceil(vec->size()/numMaps);
        //intilizes 2D vector
        std::vector< std::vector<std::string> > vects;
        int vecCurrentElement = 0;
        //the number of sub-vectors is the number of mappers loop to create a sub vector for each mapper
        for(int i = 0; i < numMaps; i++){
                //initilze a temp vector to hold subbector
                std::vector<std::string> temp;
                //first check how many subvectors get one extra element (i.e. distributing remaining elements
                //among the first n elements
                if(i < (vec->size() % numMaps)){
                        for(int t = 0; t < numElements+1; t++){
                                temp.push_back((*vec)[vecCurrentElement]);
                                vecCurrentElement++;
                        }
                }
                //otherwise just add apropriate number of elements
                else{
                        for(int t = 0; t < numElements; t++){
                                temp.push_back((*vec)[vecCurrentElement]);
                                vecCurrentElement++;
                        }
                }
                //add temp to 2D vector that will then be used to pair with threads/procs
                vects.push_back(temp);
      }

        //test printer for checking partions
        //for(int i = 0; i < vects.size(); i++){
        //      for(int t = 0; t < vects[i].size(); t++){
        //              std::cout << vects[i][t] << std::endl;
        //      }
        //      std::cout << "------------------------------" << i << std::endl;
        //}

        pid_t wpid;
        key_t key = ftok("shmfle",100);
        int shmid = shmget(key,1024,0666|IPC_CREAT);
        char *str = (char*) shmat(shmid,(void*)0,0);
        sprintf(str,"i have no idea if this works");
        //void *ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, smfd, 0);
        for(int i = 0; i < numMaps; i++){
                pid_t pid = fork();
                if(pid == 0){
                        std::vector< std::pair<std::string, int> > keyValue;
                        for(int j = 0; j < vects[i].size(); j++){
                                keyValue.push_back(make_pair(vects[i][j], 1));
                        }
                        std::cout << "chiled " << getpid() << " parent " << getppid() << std::endl;
                        for(int i = 0; i < keyValue.size(); i++){
                                //std::cout << keyValue[i].first << ", " << keyValue[i].second << std::endl;
                        }
                exit(0);
                }
        }
        int status = 0;
        while((wpid = wait(&status)) > 0);

}
