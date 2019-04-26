#include <stdio.h>
#include <stdlib.h>

int main(){

	FILE *arquivos;
	int n,c;
	int i,j,size;
	char diretorio[20];

	printf("Digite o numero de arquivos\n");
	scanf("%d",&n);

	printf("Digite o numero de candidatos\n");
	scanf("%d",&c);

	for(i=1;i<=n;i++){
		sprintf(diretorio,"%d.in",i);
		arquivos = fopen(diretorio,"w");
		size = rand() % 100;
		size++;
		for(j=0;j<size;j++){
			fprintf(arquivos,"%d\n",rand()%(c+1));
		}
		fclose(arquivos);		
	}	

	free(arquivos);
	return 0;
}
