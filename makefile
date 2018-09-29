all: threadfunc.o mapred.cpp
	g++ -o mapred mapred.cpp threadfunc.o

threadfunc.o: threadfunc.cpp
	g++ -c threadfunc.cpp

run: mapred
	./mapred wordproc procs 4 4 geo.txt

clean: mapred threadfunc.o
	rm mapred threadfunc.o
