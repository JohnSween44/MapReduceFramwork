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
#include "mapred.h"


using namespace std;

typedef struct VAL_PAIR{
        char word[5000000];
        int wordcount;
        int offset;
}memTracker;
std::vector<std::pair <std::string, int > > glb_vec;
void sortAFunc(std::vector<std::pair<std::string, int> > * sortMe);
void mapper(char **argv);
void wordCounter(vector<string>, shared_mutex_t, memTracker*);
void reducer(char** argv);
void wordCombiner(vector< pair<string, int> >, shared_mutex_t , memTracker*);

//Global 
std::vector<std::pair <std::string, int > > glb_vec;

int main(int argc, char ** argv){

mapper(argv);
reducer(argv);

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

	
        int smfd = shm_open("shared", O_CREAT | O_RDWR, 0666);
        ftruncate(smfd, sizeof(memTracker));
        memTracker* sharedstr = (memTracker*)mmap(0, sizeof(memTracker), PROT_WRITE, MAP_SHARED, smfd, 0);
        shared_mutex_t lock = shared_mutex_init("bob");
     	sharedstr->offset = 0;
        sharedstr->wordcount = 0;




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
<


	
        //test printer for checking partions
	//for(int i = 0; i < vects.size(); i++){
        //      for(int t = 0; t < vects[i].size(); t++){
        //              std::cout << vects[i][t] << std::endl;
        //      }
        //      std::cout << "------------------------------" << i << std::endl;
        //}
	

	//Want process
	if(p11 == 0){
		pid_t wpid;
        	for(int i = 0; i < num_maps; i++){
                	pid_t pid = fork();
                	if(pid == 0){
                		if(p == 0){
					//Process wordcount stuff;
					wordCounter(vects[i], lock, sharedstr);
				}
				else if (p1 == 0){
					//Process sort stuff
				}
			
               		}
        	}
       		int status = 0;
        	while((wpid = wait(&status)) > 0);
        	shared_mutex_destroy(lock);
	}
	//Want threads
	else if(p111 == 0){
		if(p == 0){
		//Process wordcount stuff;
			//pthread_mutex_t mtx;
			//pthread_t threads[maps];
			//std::vector <threadInfo *> titrack;

				
		}
		else if (p1 == 0){
			//Process sort stuff
		}
	}

}


void wordCounter(vector<string> vect, shared_mutex_t lock, memTracker *sharedstr){


        vector< pair<string, int> > keyValue;
        for(int j = 0; j < vect.size(); j++){
                keyValue.push_back(make_pair(vect[j], 1));
        }	

        for(int n = 0; n < keyValue.size(); n++){
                for(int q = 0; q < keyValue.size(); q++){
                        if(keyValue[n].first == keyValue[q].first && n != q){
                                keyValue[n].second += keyValue[q].second;
                                keyValue.erase(keyValue.begin()+q);
                                q--;
                        }
                }
        }


        for(int p = 0; p < keyValue.size(); p++){

                const char *temp = keyValue[p].first.c_str();
                char intstr[500];
                sprintf(intstr,"%d", keyValue[p].second);
                char *writedata = (char*)malloc(strlen(temp) + strlen(intstr));
                sprintf(writedata,"%s %s",temp,intstr);

                pthread_mutex_lock(lock.ptr);

                int offset = sharedstr->offset;
                sprintf(&sharedstr->word[offset]," %s", writedata);
                sharedstr->offset += (strlen(writedata)+1);
                sharedstr->wordcount++;

                pthread_mutex_unlock(lock.ptr);
        }
        exit(0);

}

void reducer(char** argv){

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

	//create shared mem
	int smfd = shm_open("shared", O_CREAT | O_RDWR, 0666);
	ftruncate(smfd, sizeof(memTracker));
	memTracker* sharedstr = (memTracker*)mmap(0, sizeof(memTracker), PROT_WRITE, MAP_SHARED, smfd, 0);
	shared_mutex_t lock = shared_mutex_init("bob");
	sharedstr->offset = 0;
	sharedstr->wordcount = 0;

	vector< pair<string, int> > pairs;
	if(p11 == 0){
		//read from shared mem
		string tempstr = sharedstr->word;
		vector<string> tokens;
		//cout << tempstr << endl;
	
		stringstream checkl(tempstr);
		string intermediate;
		while(getline(checkl, intermediate, ' ')){
			tokens.push_back(intermediate);
		}
		
		for(int r = 1; r < tokens.size()-1; r+=2){
			const char* count = tokens[r+1].c_str();
			pairs.push_back(make_pair(tokens[r],atoi(count)));
		}
	
		sortAFunc(&pairs);
		//for(int z = 0; z < pairs.size(); z++){
		//	cout << pairs[z].first << " " << pairs[z].second << endl;
		//}
	}
		
	vector< vector< pair<string, int> > > vectsOfPairs;
	//vectorize shared mem
	int l = ceil(pairs.size()/num_reduces);
	int w = 0;
	for(int a = 0; a < num_reduces; a++){
		vector< pair<string, int> > temp;
		if(a < (pairs.size() % num_reduces)){
			for(int t = 0; t < l+1; t++){
				temp.push_back(pairs[w]);
				w++;
			}
		}
		else{
			for(int t = 0; t < l; t++){
				temp.push_back(pairs[w]);
				w++;
			}	
		}
	
		vectsOfPairs.push_back(temp);
	}
	//for(int b = 0; b < vectsOfPairs.size(); b++){
	//	for(int t = 0; t < vectsOfPairs[b].size(); t++){
	//		cout << vectsOfPairs[b][t].first << " -- " << vectsOfPairs[b][t].second <<  endl;
	//	}
	//	cout << "------------------------------" << b << endl;
	//}
	
	
	//want process
	if(p11 == 0){
		pid_t wpid;
		for(int i = 0; i < num_reduces; i++){
			pid_t pid = fork();
			if(pid == 0){
				if(p == 0){
					//process wordcount
					wordCombiner(vectsOfPairs[i], lock, sharedstr);
				}
				else if(p1 == 0){
					//process sort
				}
			}	
		}
		int status = 0;
		while((wpid = wait(&status)) > 0);
	}
	//want threads
	else if(p111 == 0){
		if(p == 0){
			//process wordcount	
		}
		else if(p1 == 0){
			//process sort stuff
		}
	}
	
	if(p11 == 0){
		string tempstr2 = sharedstr->word;
		vector<string> tokens2;
	
		//read from shared mem again
		stringstream checkl2(tempstr2);
		string intermediate2;
		while(getline(checkl2, intermediate2, ' ')){
			tokens2.push_back(intermediate2);
		}
		vector< pair<string, int> > pairs2;
		for(int t = 1; t < tokens2.size()-1; t+=2){
			const char* count = tokens2[t+1].c_str();
			pairs2.push_back(make_pair(tokens2[t],atoi(count)));
		}
	
		//one more outer reduce
		sortAFunc(&pairs2);
		for(int n = 0; n < pairs2.size(); n++){
			//cout << keyValue.size() << endl;
			for(int q = 0; q < pairs2.size(); q++){
				if(pairs2[n].first == pairs2[q].first && n != q){
					pairs2[n].second += pairs2[q].second;
					pairs2.erase(pairs2.begin()+q);
					q--;
				}
			}
		}
		//print to screen/write to file
		sortAFunc(&pairs2);
		for(int j = 0; j < pairs2.size(); j++){
			cout << pairs2[j].first << " " <<  pairs2[j].second << endl;
		} 
	}
	shared_mutex_destroy(lock);
}


void wordCombiner(vector< pair<string, int> > keyValue, shared_mutex_t lock, memTracker *sharedstr){
	
	for(int n = 0; n < keyValue.size(); n++){
		for(int q = 0; q < keyValue.size(); q++){
			if((keyValue[n].first.compare(keyValue[q].first) == 0 ) && (n != q)){
				keyValue[n].second += keyValue[q].second;
				keyValue.erase(keyValue.begin()+q);
				q--;
			}
		}
	}

	
	//cout << "chiled " << getpid() << " parent " << getppid() << " reduce" << endl;
	for(int p = 0; p < keyValue.size(); p++){
		
		const char *temp = keyValue[p].first.c_str();
		char intstr[500];
		sprintf(intstr,"%d", keyValue[p].second);
		char *writedata = (char*)malloc(strlen(temp) + strlen(intstr));
		sprintf(writedata,"%s %s",temp,intstr);
	
		pthread_mutex_lock(lock.ptr);
			
		int offset = sharedstr->offset;
		sprintf(&sharedstr->word[offset]," %s", writedata);
		sharedstr->offset += (strlen(writedata)+1);
		sharedstr->wordcount++;
			
		pthread_mutex_unlock(lock.ptr);		
	} 

	exit(0);
}



void sortAFunc(std::vector<std::pair<std::string, int> > * sortMe){
	for(int i = 0; i < sortMe->size() - 1; i++){
		for(int j = 0; j < sortMe->size(); j++){
			if(sortMe->at(i).first.compare(sortMe->at(j).first) < 0 )
				std::swap(sortMe->at(i), sortMe->at(j));
		}
	}
}
