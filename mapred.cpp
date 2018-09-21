#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>


void vectorizer(std::ifstream * in, std::vector < std::string > * vectorIn);

int main(int argc, char ** argv){

	//Variable initialization
	int app;
	int impl;
	int num_maps = argv[2];
	int num_reduces = argv[3];
	string input = argv[4];
	string output = argv[5];

	//Update variables from command line
	if(argv[0] == "wordcount"){
		app = 1;
	}else if(argv[0] == "sort"){
		app = 0;
	}else{
		app = -1;
	}

	if(argv[1] == "procs"){
		impl = 1;
	}else if(argv[1] == "threads"){
		impl = 0;
	}else{
		impl = -1;
	}

	if(app == -1 || impl == -1){
		cout << "Invalid Input!";
	}else{
		
	}
	std::ifstream readin ; 
	readin.open(input);

	//Vector of pairs to read into
	std::vector < std::string > totList;

	//Tokenize  
	vectorizer(&readin, &totList);


	for (int i = 0; i < totList.size(); i++){
		std::cout << totList[i] << "\n";
	}

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
	Tokenzied vector of all the words present in file passed in to function.
*/
void vectorizers(std::ifstream * in, std::vector < std::string > * vectorIn){

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

}