#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_THREADS 1000 // criando no maximo 1000 threads na main

int contador = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER; // inicicando mutex

void *somador(void *threadid){

	int *id = (int *) threadid; // id da thread atual

	while(contador < 1000000){
		pthread_mutex_lock(&mymutex); // trava todas as outras threads
		contador++; // incrementa
		if(contador == 1000000){
			printf("Oi, sou a thread #%d ! Eu cheguei no limite!\n",*id);
		}
		pthread_mutex_unlock(&mymutex); // destrava todas as outras threads	
	}
	pthread_exit(NULL);
}

int main(){

	pthread_t threads[MAX_THREADS];
	int threadid[MAX_THREADS];
	int rc;

	for(int i=0;i<MAX_THREADS;i++){
		// cria uma nova thread
		threadid[i] = i; // distribui os ids
		if(contador >= 1000000) break; // limite para que a main pare de criar threads
		rc = pthread_create(&threads[i],NULL,somador,(void *) &threadid[i]);
		if(rc){
			printf("falha ao criar thread de id %d. Codigo #%d\n",i,rc);
			exit(-1);
		} 	
	}
	pthread_exit(NULL);
	return 0;
}
