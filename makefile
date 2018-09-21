all: sorter.cpp
	g++ -o sort sorter.cpp



clean: sort
	rm sort
