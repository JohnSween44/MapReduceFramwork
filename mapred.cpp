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
#include "mapred.h"

void sortAFunc(std::vector<std::pair<std::string, int> > * sortMe);
void mapper(char **argv);

//Global 
std::vector<std::pair <std::string, int > > glb_vec;

int main(int argc, char ** argv){


mapper(argv);
std::cout << "Stored Size: " << glb_vec.size() << std::endl;
sortAFunc(&glb_vec);

        for(int i = 0; i < glb_vec.size(); i++){
        	std::cout << glb_vec[i].first << " " << glb_vec[i].second << std::endl;	        
	}



}





void mapper(char **argv){

        //Variable initialization
        int app;
        int impl;
        int num_maps = atoi(argv[3]);
        int num_reduces = atoi(argv[4]);
        char* input = argv[5];
        char* output = argv[6];

	int p = strcmp(argv[1], "wordcount");
	int p1 = strcmp(argv[1], "sort");
	int p11 = strcmp(argv[2], "procs");
	int p111 = strcmp(argv[2], "threads");

	if(p == 0)
		app = 1;
	else if (p1 = 0)
		app = 0;
	else{
		std::cout << "Unexpected Input: \"" << argv[1] << "\"\n";
		std::exit(-1);  
	}        

	 if(p11 == 0)
                impl = 1;
        else if (p111 == 0)
                impl = 0;
        else{
                std::cout << "Unexpected Input: \"" << argv[2] << "\"\n";
                std::exit(-1);
        }


	std::ifstream readin;
	readin.open(input);
	std::vector < std::string > vec;

        std::string a;
        std::string holdMe;


        while(readin >> a){
                char temp[a.length()+1];
                strcpy(temp, a.c_str());
                char * wo = strtok(temp, " .,:;?!-");
                std::string eff = wo;
                std::transform(eff.begin(), eff.end(), eff.begin(), ::tolower);
                vec.push_back( eff );
        }


        int numElements = ceil(vec.size()/num_maps);
        //intilizes 2D vector 
        std::vector< std::vector<std::string> > vects;
        int vecCurrentElement = 0;
        //the number of sub-vectors is the number of mappers loop to create a sub vector for each mapper
        for(int i = 0; i < num_maps; i++){
                //initilze a temp vector to hold subbector
                std::vector<std::string> temp;
                //first check how many subvectors get one extra element (i.e. distributing remaining elements 
                //among the first n elements
                if(i < (vec.size() % num_maps)){
                        for(int t = 0; t < numElements+1; t++){
                                temp.push_back(vec[vecCurrentElement]);
                                vecCurrentElement++;
                        }
                }
                 //otherwise just add apropriate number of elements
                else{
                        for(int t = 0; t < numElements; t++){
                                temp.push_back(vec[vecCurrentElement]);
                                vecCurrentElement++;
                        }
                }
                //add temp to 2D vector that will then be used to pair with threads/procs
                vects.push_back(temp);
        }


/*	
        //test printer for checking partions
        for(int i = 0; i < vects.size(); i++){
                for(int t = 0; t < vects[i].size(); t++){
                        std::cout << vects[i][t] << std::endl;
                }
                std::cout << "------------------------------" << i << std::endl;
        }
*/	

	//Want process
	if(p11 == 0){
		//Want wordcount
		if(p == 0){
			//Process wordcount stuff;
		}
		else if (p1 == 0){
			//Process sort stuff
		}

	}
	//Want threads
	else if(p111 == 0){
		if(p == 0){
		//Process wordcount stuff;
			pthread_mutex_t mtx;
			pthread_mutex_init(&mtx, NULL);
			pthread_t threads[num_maps];
			std::vector <threadInfo *> titrack;
			for (int i = 0; i < num_maps; i++){
				threadInfo * tempti = new threadInfo();
				tempti->readFrom = &vects[i];
				tempti->mutex = &mtx;
				titrack.push_back(tempti);
		        	pthread_t temp;
				threads[i] = temp;
        			
				int a = pthread_create(&threads[i], NULL, &threadWorker, (void *)tempti);

        			if (a != 0)
                			printf("fail\n");
				}
	
			for (int i = 0; i < num_maps; i++)
        			pthread_join(threads[i], (void **)NULL);

			for (int i = 0; i < num_maps; i++)
				delete(titrack[i]);
		}//end if



		else if (p1 == 0){
			//Thread sort stuff
		}
	}

}



void reducer(char ** argv){



}


void sortAFunc(std::vector<std::pair<std::string, int> > * sortMe){

for(int i = 0; i < sortMe->size() - 1; i++){
	for(int j = 0; j < sortMe->size(); j++){
		if((sortMe->at(i).first.compare(sortMe->at(j).first) <  0) && i != j )
			std::swap(sortMe->at(i), sortMe->at(j));
	}
}


}
