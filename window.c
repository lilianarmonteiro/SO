#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "readln.h"
#include "procura.h"
#include "addFimLinha.h"

#include <stdio.h>

int myWindow(char* buf, int qt, char** arrayLinhas, int linhas, int linhasAntigas, char* coluna, char* operador){
    int l, i;
    int elemInt[linhas-1]; //-1 porque a primeira linha nao é necessaria para os calculos
    char* elem[linhas-1];

	int soma, max, min;
	int media;
    char str[PIPE_BUF];
   	int qtConst;

   	for(i=0; i<linhas-1; i++){
    	elem[i] = malloc(PIPE_BUF * sizeof(char*));
    	memset(elem[i], 0, PIPE_BUF);
    	elemInt[i]=0;
    }
 
    for(l=1; l<=linhasAntigas; l++){
    	elem[l-1] = procura(arrayLinhas[l], qt, coluna);
    	elemInt[l-1] = atoi(elem[l-1]);
	}

	for(i=0; i<linhas-1; i++) free(elem[i]);

	soma=0;
	for(i=0; i<linhasAntigas; i++){
		soma += elemInt[i];
	}

	max=elemInt[0];
	for(i=1; i<linhasAntigas; i++){
		if(elemInt[i]>max) max=elemInt[i];
	}
	
	min=elemInt[0];
	for(i=1; i<linhasAntigas; i++){
		if(elemInt[i]<min) min=elemInt[i];
	}

    if((strcmp(operador, "avg")) == 0){
        if(soma==0) media = soma;
        else media = (int) soma/linhasAntigas;
        sprintf(str, "%d", media);
        qtConst = addFimLinha(buf, qt, str);
   		return qtConst;
    }
    if((strcmp(operador, "max")) == 0){
    	sprintf(str, "%d", max);
        qtConst = addFimLinha(buf, qt, str);
    	return qtConst;
    }
    if((strcmp(operador, "min")) == 0){
    	sprintf(str, "%d", min);
        qtConst = addFimLinha(buf, qt, str);
    	return qtConst;
    } 
    if((strcmp(operador, "sum")) == 0){
    	sprintf(str, "%d", soma);
 		qtConst = addFimLinha(buf, qt, str);
    	return qtConst;
    }

	return 0;
}

int main(int argc, char** argv){
	char buf[PIPE_BUF];
	int qt, qtWindow, i, linhas, linhasAntigas=0;

    if(argc!=4){
        printf("Número de argumentos de window inválido\n");
        return -1;
    }
    
    linhas = atoi(argv[3])+1;
    char* arrayLinhas[linhas];

    for(i=0;i<linhas;i++) {
    	arrayLinhas[i] = malloc(PIPE_BUF * sizeof(char*));
    	memset(arrayLinhas[i], 0, PIPE_BUF);
    }

	while((qt=readln(0,buf,PIPE_BUF))>0){
        strcpy(arrayLinhas[0],buf);

        qtWindow = myWindow(buf, qt, arrayLinhas, linhas, linhasAntigas, argv[1], argv[2]);
        write(1, buf, qtWindow);
     	memset(buf, 0, qtWindow);

        for(i=linhas-2; i>=1; i--){
        	strcpy(arrayLinhas[i+1],arrayLinhas[i]);
        }
        strcpy(arrayLinhas[1],arrayLinhas[0]);    	
		
        if(linhas-1 > linhasAntigas) linhasAntigas++;
	}

	for(i=0; i<linhas; i++) free(arrayLinhas[i]);
	return 0;
}