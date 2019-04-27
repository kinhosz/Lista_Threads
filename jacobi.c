#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define P 10 // definindo a precisao como 10
#define LIM 8 // definindo o limite de incognitas como 8

int matriz[LIM+1][LIM+1]; // matriz com os valores
int ans[LIM+1]; // respostas das equacoes lineares
double incog[P+1][LIM+1]; // incognitas
int n; // qtd de threads

pthread_barrier_t barrier; // cria uma barreira


// setando valores iniciais
void seta(){

  int i,j;

  for(i=0;i<LIM+1;i++){
    for(j=0;j<LIM+1;j++){
      matriz[i][j] = 2;
    }
    ans[i] = 5;
  }

  for(i=0;i<P+1;i++){
    for(j=0;j<LIM+1;j++){
      incog[i][j] = 1;
    }
  }
}


void *refina(void *threadid){


  int *in = (int *) threadid;
  int id = *in;
  int i,j,k; // contadores
  long double value; // variavel que recebera o valor atual
  int teto;
  long double soma = 0;

  // definindo o limite das funcoes
  if(id == n-1) teto = LIM;
  else teto = (id+1)*(LIM/n);

  // algoritmo que ira refinar as incognitas P vezes
  for(i=1;i<=P;i++){
      for(j=id*(LIM/n);j<teto;j++){
        value = 0;
        value = ((long double)1/(matriz[j][j])); // implementando a formula
        soma = 0;
        for(k=0;k<LIM;k++){
            if(j != k){
              // implementacao da formula
              soma += (long double)(matriz[j][k] * incog[i-1][k]);
            }
        }
        soma = (long double)(ans[j] - soma); // implementacao da formula
        value = (long double)value*soma;
        // value assume o valor da variavel refinada
        incog[i][j] = value;
      }
      // apos terminar sua tarefa, eh necessario que ele espere as outras threads acabarem
      pthread_barrier_wait(&barrier);
  }
  pthread_exit(NULL);
}



int main(){


    pthread_t *thread;
    int *threadid;
    int i;


    seta();

    printf("Digite o numero de threads:\n");
    scanf("%d",&n);

    // allocando memoria para as threads
    thread = (pthread_t *) malloc(n*sizeof(pthread_t));
    threadid = (int *) malloc(n*sizeof(int));
    // iniciando barreira
    pthread_barrier_init(&barrier,NULL,n); 

    for(i=0;i<n;i++){
        threadid[i] = i;
        pthread_create(&thread[i],NULL,refina,(void *) &threadid[i]);
    }
    // espera todas as threads finalizarem
    for(i=0;i<n;i++){
      pthread_join(thread[i],NULL);
    }
    printf("valores das incognitas apos P interacoes\n");
    for(i=0;i<LIM;i++){
      printf("%lf ",incog[P][i]);
    }
    printf("\n");

    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
    free(thread);
    free(threadid);
    return 0;
}
