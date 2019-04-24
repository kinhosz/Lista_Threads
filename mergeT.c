#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{

    int l,r;
    int id;
    // l = indice inicial do vetor
    // r = indice final do vetor
}DATA;

typedef struct{

    int l,r,pivo; // argumento para a segunda funcao do merge 
}DATA2;

// declarando um tamanho qualquer, vetor.size = 10^5
int vetor[100005];

// parte do merge que une duas partes do vetor
void *mergeSort(void *input){
    
    int l,r,pivo; // valores lindos aqui
    int k = 0; // contador
    int *aux; // vetor auxiliar
    int size; // tamanho do vetor
    int i,j;
    DATA2 *argumento;
    // recebendo e passando parametros
    argumento = (DATA2 *) input;
    l = argumento->l;
    pivo = argumento->pivo;
    r = argumento->r;
    i = l;  // valor para meu two_pointers
    j = pivo+1; // valor para meu two_pointers

    size = (r - l + 1);
    aux = (int *) malloc(size * sizeof(int));


    k = 0; // contador
    while(i <= pivo && j <= r){
        if(vetor[i] <= vetor[j]){
            aux[k++] = vetor[i++];
        }
        else{
            aux[k++] = vetor[j++];
        }
    }

    while(i<=pivo){
        aux[k++] = vetor[i++];
    }
    
    while(j<=r){
        aux[k++] = vetor[j++];
    }
    // repassando todos os valores para o vetor
    for(i=0;i<k;i++){
        vetor[l++] = aux[i];
    }
    // libera memoria alocada
    free(aux);
    pthread_exit(NULL);

}

// funcao pai que cria duas Threads
void *merge(void *input){

    pthread_t thread1,thread2,thread3; // criando duas threads, uma para cada parte de um vetor
    DATA *entrada; // estrutura que recebera a entrada
    DATA *saida1,*saida2; // estruturas para passar parametro para a outra parte do merge
    DATA2 *arg; // argumento para segunda funcao do merge
    int l,r,pivo; // variaveis auxiliares
    int rc1,rc2,rc3; // valores para verificar o status
    int id; // id da thread atual
    entrada = ((DATA *) input);
    // dividindo os indices
    l = entrada->l;
    r = entrada->r;
    id = entrada->id;
    pivo = (l+r)/2;
    // alocando memoria para as estruturas de argumento da funcao
    saida1 = (DATA *) malloc(sizeof(DATA));
    saida2 = (DATA *) malloc(sizeof(DATA));
    arg = (DATA2 *) malloc(sizeof(DATA2));
    // distribuindo os valores do argumento
    saida1->l = l;
    saida1->r = pivo;
    saida1->id = 2*id;
    saida2->l = pivo+1;
    saida2->r = r;
    saida2->id = 2*id + 1;
    // argumento da funcao 'mergeSort'
    arg->l = l;
    arg->r = r;
    arg->pivo = pivo;
    //printf("Estou na thread %d\n",id); // debugador
    if(l != r){
        rc1 = pthread_create(&thread1,NULL,merge,(void *)saida1);
        if(rc1){
            printf("Falha ao criar Thread1 na recursao. Codigo #%d\n",rc1);
            exit(-1);
        }
        rc2 = pthread_create(&thread2,NULL,merge,(void *)saida2);
        if(rc2){
            printf("Falha ao criar Thread2 na recursao. Codigo #%d\n",rc2);
        }

        // a Thread responsavel por esta instrucao so continuara quando as duas
        // threads criadas por elas voltarem.
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
        // agora, com duas partes ordenadas, vamos ordena-las entre si
        rc3 = pthread_create(&thread3,NULL,mergeSort,(void *) arg);
        if(rc3){
            printf("Falha ao criar thread para unir as duas partes. Codigo #%d\n",rc3);
            exit(-1);
        }
        // espera a thread3 finalizar seu trabalho
        pthread_join(thread3,NULL);
        // parte ordenadaa =)
    }

    // libera memoria alocada
    free(saida1);
    free(saida2);
    free(arg);
    pthread_exit(NULL);
}

int main(){

    pthread_t thread_Main; // Thread da Main
    DATA *initial; // valor para passar parametros iniciais

    int n,i,rc;
    // n = limite
    // i = contador
    // rc = status da Thread

    // leitura de dados
    scanf("%d",&n);
    for(i=0;i<n;i++){
        scanf("%d",&vetor[i]);
    }
    initial = (DATA *) malloc(sizeof(DATA));
    initial->l = 0; // recebe o parametro inicial
    initial->r = n-1; // recebe o parametro final
    initial->id = 1; // so para saber quem esta na CPU no mometo
    rc = pthread_create(&thread_Main,NULL,merge,(void *) initial);
    if(rc){
        printf("Falha ao criar Thread na Main. Codigo de verificacao #%d\n",rc);
        exit(-1);
    }
    // aguarda a Thread da Main (criada pela Main) acabar
    pthread_join(thread_Main,NULL);
    // como a thread da Main ja acabou, podemos retornar.
    printf("vetor ordenado!\n");
    for(i=0;i<n;i++){
        printf("%d%c",vetor[i],i==n-1?'\n':' ');
    }

    pthread_exit(NULL); // aguarda todas as Threads serem interrompidas
    free(initial); // libera memoria
    // como usei o join, a unica thread criada ja foi interrompida
 
    return 0;
}