

int main(int argc, char* argv[]){
	
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

	return 0;
}

