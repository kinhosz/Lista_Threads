#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct{

    int linha_inicial;
    int linha_final;
    int coluna_inicial;
    int coluna_final;
    int id; // id da thread

}DATA;

int grid[10005][10005];

void *bitmap(void *Arg){

    // declaracao dos valores para receber o argumento
    DATA *input;
    input = (DATA *) Arg;
    char *ret; // string para retorno
    int linhai; // linha inicial
    int linhaf; //linha final
    int colunai; // coluna inicial
    int colunaf; // coluna final
    int id; // id da thread

    // passagem dos valores
    linhai = input->linha_inicial;
    linhaf = input->linha_final;
    colunai = input->coluna_inicial;
    colunaf = input->coluna_final;
    id = input->id;

    // teste da thread
    //printf("Oi, eu sou a thread #%d\n",id);

    int value = grid[linhai][colunai]; // recebe o valor da primeira posicao da matriz
    int dif = 0;
    int i,j;
    int idx = 0; // idx eh o indice

    //retorno das threads filhas
    char *retorno1, *retorno2, *retorno3, *retorno4;
    // threads filhas
    pthread_t thread1,thread2,thread3,thread4;
    // status de retorno da criacao das threads
    int s1,s2,s3,s4;
    int meio,meio2;
    // argumentos das funcoes
    DATA *A1,*A2,*A3,*A4; 
    // alocando memoria
    A1 = (DATA *) malloc(sizeof(DATA));
    A2 = (DATA *) malloc(sizeof(DATA));
    A3 = (DATA *) malloc(sizeof(DATA));
    A4 = (DATA *) malloc(sizeof(DATA));

    // alocando memoria para a string de retorno
    ret = (char *) malloc(100*sizeof(char));

    for(i=linhai;i<=linhaf;i++){
        for(j=colunai;j<=colunaf;j++){
            if(value != grid[i][j]){
                dif = 1;
                break;
            }
        }
    }
    if(dif){
        // preciso colocar 'D' no retorno e chamar as 4 threads para o tabuleiro
        strcpy(ret,"D");
        // so existe uma linha, logo tera 2 threads
        if(linhai == linhaf){
            meio = (colunai+colunaf)/2;
            // passando os argumentos para A1
            A1->id = id*4;
            A1->linha_inicial = linhai;
            A1->linha_final = linhaf;
            A1->coluna_inicial = colunai;
            A1->coluna_final = meio; // pega a metade da coluna
            // passando os argumentos para A2
            A2->id = id*4 + 1;
            A2->linha_inicial = linhai;
            A2->linha_final = linhaf;
            A2->coluna_inicial = meio+1; //pega a segunda metade da coluna
            A2->coluna_final = colunaf;

            // criando a primeira thread
            s1 = pthread_create(&thread1,NULL,bitmap,(void *) A1);
            if(s1){
                printf("Erro ao criar thread! Codigo #%d\n",s1);
                exit(-1);
            }
            // criando a segunda thread
            s2 = pthread_create(&thread2,NULL,bitmap,(void *) A2);
            if(s2){
                printf("Erro ao criar thread! Codigo #%d\n",s2);
                exit(-1);
            }
            // espera as duas threads acabarem
            pthread_join(thread1,(void **)&retorno1);
            pthread_join(thread2,(void **)&retorno2);

            // concatena as strings
            strcat(ret,retorno1);
            strcat(ret,retorno2);
            // libera as strings usadas
            free(retorno1);
            free(retorno2);
        }
        else if(colunai==colunaf){
            meio = (linhai+linhaf)/2;
            // passando os argumentos para A1
            A1->id = id*4;
            A1->linha_inicial = linhai;
            A1->linha_final = meio;
            A1->coluna_inicial = colunai;
            A1->coluna_final = colunaf; // pega a metade da coluna
            // passando os argumentos para A2
            A2->id = id*4 + 1;
            A2->linha_inicial = meio+1;
            A2->linha_final = linhaf;
            A2->coluna_inicial = colunai; //pega a segunda metade da coluna
            A2->coluna_final = colunaf;

            // criando a primeira thread
            s1 = pthread_create(&thread1,NULL,bitmap,(void *) A1);
            if(s1){
                printf("Erro ao criar thread! Codigo #%d\n",s1);
                exit(-1);
            }
            // criando a segunda thread
            s2 = pthread_create(&thread2,NULL,bitmap,(void *) A2);
            if(s2){
                printf("Erro ao criar thread! Codigo #%d\n",s2);
                exit(-1);
            }
            // espera as duas threads acabarem
            pthread_join(thread1,(void **)&retorno1);
            pthread_join(thread2,(void **)&retorno2);

            // concatena as strings
            strcat(ret,retorno1);
            strcat(ret,retorno2);
            // libera as strings usadas
            free(retorno1);
            free(retorno2);
        }
        else{
            meio = (linhai+linhaf)/2;
            meio2 = (colunai+colunaf)/2;
            // bloco superior da esquerda
            A1->id = id*4;
            A1->linha_inicial = linhai;
            A1->linha_final = meio;
            A1->coluna_inicial = colunai;
            A1->coluna_final = meio2;
            // bloco superior da direita
            A2->id = id*4 + 1;
            A2->linha_inicial = linhai;
            A2->linha_final = meio;
            A2->coluna_inicial = meio2+1;
            A2->coluna_final = colunaf;
            // bloco inferior esquerdo
            A3->id = id*4 + 2;
            A3->linha_inicial = meio+1;
            A3->linha_final = linhaf;
            A3->coluna_inicial = colunai;
            A3->coluna_final = meio2;
            // bloco inferior direito
            A4->id = id*4 + 3;
            A4->linha_inicial = meio+1;
            A4->linha_final = linhaf;
            A4->coluna_inicial = meio2+1;
            A4->coluna_final = colunaf;

            // criando threads
            s1 = pthread_create(&thread1,NULL,bitmap,(void *) A1);
            if(s1){
                printf("Erro ao criar thread. Codigo #%d\n",s1);
                exit(-1);
            }
            s2 = pthread_create(&thread2,NULL,bitmap,(void *) A2);
            if(s2){
                printf("Erro ao criar thread. Codigo #%d\n",s2);
                exit(-1);
            }
            s3 = pthread_create(&thread3,NULL,bitmap,(void *) A3);
            if(s3){
                printf("Erro ao criar thread. Codigo #%d\n",s3);
                exit(-1);
            }
            s4 = pthread_create(&thread4,NULL,bitmap,(void *) A4);
            if(s4){
                printf("Erro ao criar thread. Codigo #%d\n",s4);
                exit(-1);
            }

            // espera as threads terminarem
            pthread_join(thread1,(void **) &retorno1);
            pthread_join(thread2,(void **) &retorno2);
            pthread_join(thread3,(void **) &retorno3);
            pthread_join(thread4,(void **) &retorno4);

            // concatena as strings
            strcat(ret,retorno1);
            strcat(ret,retorno2);
            strcat(ret,retorno3);
            strcat(ret,retorno4);

            // libera as strings usadas
            free(retorno1);
            free(retorno2);
            free(retorno3);
            free(retorno4);
        }

    }
    else{
        // copia valor
        if(value==1) strcpy(ret,"1");
        else strcpy(ret,"0");
    }

    // libera a memoria
    free(A1);
    free(A2);
    free(A3);
    free(A4);

    pthread_exit((void *) ret);
}

int main(){

    pthread_t thread_Main;
    DATA *datain;
    int n,m;
    int i,j;
    int rc;
    char leitor[1005];

    scanf("%d %d",&n,&m);

    for(i=0;i<n;i++){
        scanf(" %s",leitor);
        for(j=0;j<m;j++){
            grid[i][j] = leitor[j] - 48;
        }
    }

    // setando os valores
    datain = (DATA *) malloc(sizeof(DATA));
    datain->linha_inicial = 0;
    datain->coluna_inicial = 0;
    datain->linha_final = n-1;
    datain->coluna_final = m-1;
    datain->id = 1;
    // criando thread
    rc = pthread_create(&thread_Main,NULL,bitmap,(void *) datain);
    if(rc){
        printf("Erro ao criar Thread. Codigo #%d\n",rc);
        exit(-1);
    }
    char *resp; // string para guardar a resposta da execucao
    pthread_join(thread_Main,(void **)&resp); // retorna uma string

    printf("%s\n",resp); // retorna o valor do bitmap

    pthread_exit(NULL);

    free(datain); // liberando memoria

    return 0;
}