#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>


void fileReader(std::ifstream * in, std::vector < std::string > * vectorIn);

int main(int argc, char ** argv){

	//Temporary check for correct number of arguments. 
	// ***** WILL NEED TO CHANGE FOR FINAL PROGRAM ***** 
	if(argc < 2 ){
		std::cout << "Invalid number of parameters\n";
		return -1;
	}

	//Open Input Stream
	std::ifstream readin ; 
	readin.open(argv[1]);

	//Vector of pairs to read into
	std::vector < std::string > totList;

	//Tokenize  
	fileReader(&readin, &totList);


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
void fileReader(std::ifstream * in, std::vector < std::string > * vectorIn){

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

