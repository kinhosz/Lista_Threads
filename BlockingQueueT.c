#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// declaracao dos limite
#define NUM_CONSUMIDOR 4
#define NUM_PRODUTOR 4
#define SIZE_BUFFER 100
#define NUM_ITENS 4000
#define NUM_TASK 1000 // numero de tarefas (use num_itens/NUM_produtor)
// Eu considerei que NUM_CONSUMIDOR = NUM_PRODUTOR

/**************************************
Cada thread pega uma faixa de elementos

***************************************/

// declarando as condicoes
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
// declarando os mutexes
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

// declaracao das estrututuras de dados necessarias
typedef struct elem{

	int value;
	struct elem *prox;
}Elem;

typedef struct blockingQueue{

	unsigned int sizeBuffer, statusBuffer;
	Elem *head,*last; 
}BlockingQueue;

// ponteiro global para uma blockingQueue
BlockingQueue *myQueue;


// funcao que cria a fila de prioridade
BlockingQueue* newBlockingQueue(unsigned int size){

	BlockingQueue *temp;

	temp = (BlockingQueue *) malloc(sizeof(BlockingQueue));
	temp->sizeBuffer = size;
	temp->statusBuffer = 0; // fila vazia
	temp->head = NULL;
	temp->last = NULL;

	return temp;
}

void push(int elemento){

	Elem *temp,*aux;
	pthread_mutex_lock(&mymutex);
	// a fila esta cheia, logo nao posso adicionar ninguem por enquanto
	while(myQueue->statusBuffer == myQueue->sizeBuffer){
		pthread_cond_wait(&empty,&mymutex);
	}
	// se chegou aqui, entao eh possivel adicionar elementos
	// aloca memoria para pushar
	temp = (Elem *) malloc(sizeof(Elem));
	temp->value = elemento;
	temp->prox = NULL;
	// joga o novo node na fila
	if(myQueue->statusBuffer == 0){ // caso nao tenha ninguem na fila, adiciona no head e no last
		myQueue->head = temp;
		myQueue->last = temp;
		myQueue->statusBuffer = (myQueue->statusBuffer + 1);
	}
	else{ // a fila nao esta vazia, entao adiciona no fim
		aux = myQueue->last; // aux aponta para o ultima elemento
		aux->prox = temp; // o ultimo elemento agora para temp (vira penultimo)
		myQueue->last = temp; // o ultimo elemento agora eh o temp
		myQueue->statusBuffer = (myQueue->statusBuffer + 1);
	}
	// informando ao usuario
	printf("Push no elemento #%d\n",elemento);
	// como adicionado um elemento, eh possivel remover qualquer elemento
	// entao sinalizamos as threads que ja eh possivel escolher
	if(myQueue->statusBuffer > 0){
		pthread_cond_broadcast(&fill); // avisa que ja posso remover elementos
	}
	pthread_mutex_unlock(&mymutex);
}

// funcao produtora
void *produzir(void *task){

	int *in = (int *) task;
	int id = *in;
	int i; // contador

	for(i=0;i<NUM_TASK;i++){
		push(id+i); // adicionando novo elemento na fila 
	}
	pthread_exit(NULL);
}


// pop no primeiro elemento da fila
int pop(){

	int ret;
	Elem *temp;

	pthread_mutex_lock(&mymutex);
	// se a fila estiver vazia, nao eh possivel dar pop
	while(myQueue->statusBuffer == 0){
		pthread_cond_wait(&fill,&mymutex); // espera alguma thread adicionar algum elemento
	}
	// se chegou aqui, entao existe algum elemento
	temp = myQueue->head; // temp aponta para o primeiro elemento
	myQueue->head = temp->prox; // o primeiro elemento eh o segundo agora
	ret = temp->value; // ret recebe o valor que estava armazenado
	if(myQueue->statusBuffer == 1){
		myQueue->last = NULL; // irei remover o ultimo elemento. Entao last devera ser NULL
	}
	myQueue->statusBuffer = (myQueue->statusBuffer - 1);
	free(temp); // libera a memoria que deu pop

	// como eu ja removi um no, eu posso adicionar mais um elemento
	// portanto sinalizo
	if(myQueue->statusBuffer < myQueue->sizeBuffer){
		pthread_cond_broadcast(&empty);
	}
	pthread_mutex_unlock(&mymutex);

	return ret;
}


// funcao para as threads consumidoras
void *consumir(){

	int i,v;
	// essa funcao nao tem entradas, logo so ira dar pop na fila
	for(i=0;i<NUM_TASK;i++){
		v = pop();
		printf("Pop no elemento #%d\n",v);
	}
	pthread_exit(NULL);
}

int main(){

	// declarando os vetores de threads
	pthread_t producer[NUM_PRODUTOR];
	pthread_t consumer[NUM_CONSUMIDOR];

	int i;
	int taskid[NUM_PRODUTOR]; // inicio de cada posicao


	// criando a queue
	myQueue = newBlockingQueue(SIZE_BUFFER);

	// criando threads consumidoras
	for(i=0;i<NUM_PRODUTOR;i++){
		taskid[i] = i*NUM_TASK; // cada thread adicionara apenas 100 elementos na fila
		pthread_create(&producer[i],NULL,produzir,(void *) &taskid[i]);
	}
	for(i=0;i<NUM_CONSUMIDOR;i++){
		pthread_create(&consumer[i],NULL,consumir,NULL); // criando threads consumidoras
	}

	// aguardando todas as threads acabarem
	for(i=0;i<NUM_PRODUTOR;i++){
		pthread_join(producer[i],NULL);
	}
	for(i=0;i<NUM_CONSUMIDOR;i++){
		pthread_join(consumer[i],NULL);
	}

	pthread_exit(NULL);

	free(myQueue);
	return 0;
}
