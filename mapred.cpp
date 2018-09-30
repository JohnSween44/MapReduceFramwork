#include "mapred.h"


std::vector <std::vector <std::string> > vectorizer(std::ifstream *in, std::vector < std::string > *vectorIn ,int numMaps, int numReducers);

int main(int argc, char ** argv){

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

        //Vector of pairs to read into
        std::vector < std::string > totList;
	
	//Where tokenzied strings will live
	std::vector< std::vector <std::string > > storedList = vectorizer(&readin, &totList, num_maps, num_reduces); ;	
	
	//closing input stream
	readin.close();

	//Where paired...pairs will live
	std::vector< std::pair<std::string, int> > pairedValues;

	std::sort(pairedValues.begin(), pairedValues.end());
		
	if(impl == 0){
		threadMap(storedList, &pairedValues, storedList.size()); 
		printf("storedSize: %d\n", pairedValues.size());
		for(int i = 0; i < pairedValues.size(); i++){
			std::cout << pairedValues[i].first << ", " << pairedValues[i].second << std::endl;	
		}



		//Reduce w threads
	}
	else if (impl == 1){
		//Map w procs
		//Reduce w procs
	}


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
std::vector < std::vector <std::string> > vectorizer(std::ifstream *in, std::vector < std::string > *vectorIn ,int numMaps, int numReducers){

        //Open file input stream and set local vars for input and vector;
        std::ifstream * mapRead = in;
        std::vector < std::string > * vec = vectorIn;

        std::string a;
        std::string holdMe;
        
/*

	while(std::getline(*in, holdMe)){
                a += holdMe;
                a += "\n";
        }



        char * readable = new char[a.length()+1];
        strcpy (readable, a.c_str());

	//std::cout << readable << std::endl;	
        //std::cout << readable;

        char * pleaseWork = strtok(readable, " .,;:!-\n—");

        while(pleaseWork != NULL){	
	        std::string a = pleaseWork;
	
		//std::cout << pleaseWork << std::endl;
		//std::cout << a << std::endl;
		std::transform(a.begin(), a.end(), a.begin(), ::tolower);
                //std::cout << a << std::endl;
		vec->push_back( a );
                pleaseWork  = strtok(NULL, " .,:;!-\n—");
        }

*/
        while(*in >> a){
                char temp[a.length()+1];
                strcpy(temp, a.c_str());
                char * wo = strtok(temp, " .,:;?!--");
                std::string eff = wo;
                std::transform(eff.begin(), eff.end(), eff.begin(), ::tolower);
                vec->push_back( eff );

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
	return vects;
/*
        //test printer for checking partions
        for(int i = 0; i < vects.size(); i++){
                for(int t = 0; t < vects[i].size(); t++){
                        std::cout << vects[i][t] << std::endl;
                }
                std::cout << "------------------------------" << i << std::endl;
        }


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
                     //           for(int i = 0; i < keyValue.size(); i++){
                                        //std::cout << keyValue[i].first << ", " << keyValue[i].second << std::endl;
                       //         }
                        exit(0);
                        }
                }
                int status = 0;
                while((wpid = wait(&status)) > 0);

*/
}


