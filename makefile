all: mapred.cpp
	g++ -o map mapred.cpp



clean: map
	rm map
