all: threadfunc.o mapred.cpp
	g++ -o mapred mapred.cpp threadfunc.o

thread: threadfunc.o mapred.cpp
	g++ -o mapred mapred.cpp threadfunc.o -lpthread


threadfunc.o: threadfunc.cpp
	g++ -c threadfunc.cpp -lpthread

runproc: mapred
	./mapred wordcount procs 4 4 geo.txt

runthread: mapred
	./mapred wordcount threads 4 4 geo.txt

clean: mapred threadfunc.o
	rm mapred threadfunc.o
