#include "mapred.h"

typedef struct _threadInfo {
	std::vector <std::string >  * readFrom;
	std::vector <std::pair <std::string, int > > * writeTo;
}threadInfo;


pthread_mutex_t mtx;


void * threadWorker(void *arg){
threadInfo * tw = (threadInfo *)arg;

std::vector <std::string > tempRead = *tw->readFrom;
/*

std::vector <std::pair <std::string, int > >  tempWrite = *tw->writeTo;

printf("in thread: %d\n", tw->writeTo);
printf("start of write: %d\n", &tempWrite);

for(int i = 0; i < tempRead.size(); i++){
//	std::cout << tempRead[i]<<  "\n";
	pthread_mutex_lock(&mtx);
	tempWrite.push_back(make_pair(tempRead[i], 1));	
	//std::cout << tempWrite[i].first << ", " << tempWrite[i].second << std::endl;
	pthread_mutex_unlock(&mtx);
}

*/

std::vector <std::pair <std::string, int > > *  tempWrite = tw->writeTo;


for(int i = 0; i < tempRead.size(); i++){
     	//std::cout << tempRead[i]<<  "\n";
        pthread_mutex_lock(&mtx);
        tempWrite->push_back(make_pair(tempRead[i], 1));
       // std::cout << tempWrite[i].first << ", " << tempWrite[i].second << std::endl;
        pthread_mutex_unlock(&mtx);
}





pthread_exit(NULL);
}






void threadMap(std::vector< std::vector <std::string > > vectorIn, std::vector <std::pair <std::string, int > > * saveTo, int maps){

pthread_mutex_init(&mtx, NULL);
std::vector <pthread_t> threadpool;
pthread_t threads[maps];

/*
pthread_t temp;
threadInfo * ti = new threadInfo;
ti->readFrom = &vectorIn[0];
ti->writeTo = (std::vector <std::pair <std::string, int > > *)&saveTo;
ti->len = vectorIn[0].size();
int a = pthread_create(&temp, NULL, &threadWorker, (void*)ti);
pthread_join(temp,(void **) NULL);
*/


for (int i = 0; i < maps; i++){
		
	pthread_t temp;
	threads[i] = temp;
	threadInfo * ti = new threadInfo;
	ti->readFrom = &vectorIn[i];
	ti->writeTo = saveTo;
	

	int a;
	a = pthread_create(&threads[i], NULL, &threadWorker, (void * )ti);

	if (a != 0)
		printf("fail\n");	


}

for (int i = 0; i < maps; i++)
	pthread_join(threads[i], (void **)NULL);



}
