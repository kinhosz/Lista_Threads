#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// criando ponteiro de mutex (para poder alocar futuramente
pthread_mutex_t *mymutex;
// criando ponteiro com o valor dos candidatos para poder votar futuramente
// urna[x] = votos para o candidato x
int *urna;
int LIM; // maior valor do N atribuido na main
int next = 1; // next guardara o indice da proxima tarefa

// mutex para busca de tarefas
pthread_mutex_t especial =  PTHREAD_MUTEX_INITIALIZER;

void *contagem(void *arquivoid){

	int *in = (int *) arquivoid;
	int id = *in; // id contem o id do arquivo
	FILE *arq;
	char diretorio[200];
	int voto;
	int i;

	while(id <= LIM){
		// copia o endereco do diretorio (considerando que estao na mesma pasta)
		sprintf(diretorio,"%d.in",id);
		arq = fopen(diretorio,"r");
		while(fscanf(arq,"%d\n",&voto) != EOF){
			pthread_mutex_lock(&mymutex[voto]); // trava todas as threads que vao para este endereco
			urna[voto]++; // contabiliza mais um voto
			pthread_mutex_unlock(&mymutex[voto]); // desbloqueia 
		}
		fclose(arq); // fecha arquivo
		// procurando um proximo arquivo
		if(next > LIM){
			id = next; // fim das tarefas
		}
		else{
			pthread_mutex_lock(&especial); // bloqueia todas as threads so na distribuicao de tarefas
			id = next;
			next++;
			pthread_mutex_unlock(&especial); // desbloqueia todas as threads so na distribuicao de tarefas
		}
		
	}

}

int main(){

	int n,c,t; // input
	int i;
	pthread_t *threads; // pretendo ter a liberdade de alocar dinamicamente
	int *task;	// vetor que guardara a tarefa de cada thread


	printf("Digite a quantidade de arquivos para ser lido:\n");
	scanf("%d",&n);
	printf("Digite a quantidade de threads:\n");
	scanf("%d",&t);
	printf("\nDigite a quantidade de candidatos:\n");
	scanf("%d",&c);

	// allocando memoria para os candidatos e zerando
	urna = (int *) calloc(c+3,sizeof(int));
	// alocando memoria para as threads
	threads = (pthread_t *) malloc((t+3) * sizeof(pthread_t));
	// alocando memoria para os mutexes
	mymutex = (pthread_mutex_t *) malloc((c+3) * sizeof(pthread_mutex_t));
	// alocando as tarefas
	task = (int *) malloc((t+3)*sizeof(int));

	// inicializando todos os mutexes
	for(i=0;i<=c;i++){
		pthread_mutex_init(&mymutex[i],NULL);
	}

	// criando threads e passando para cada uma um arquivo inicialmente
	LIM = n; // criando limite
	for(i=0;i<t;i++){
		pthread_mutex_lock(&especial);
		task[i] = next;
		next++;
		pthread_create(&threads[i],NULL,contagem,(void *) &task[i]);
		pthread_mutex_unlock(&especial); // desbloqueia todas as threads so na distribuicao de tarefas
	}
	// espera todas as threads acabarem
	for(i=0;i<t;i++){
		pthread_join(threads[i],NULL);
	}
	// busca o campeao da eleicao
	int max = -1;
	int win = -1;
	for(i=1;i<=c;i++){
		if(urna[i] > max){
			max = urna[i];
			win = i;
		}
	}
	printf("\no vencedor foi o candidato #%d\n",win);

	pthread_exit(NULL);	

	return 0;
}
