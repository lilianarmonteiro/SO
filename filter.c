#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "readln.h"
#include "procura.h"

#include <stdio.h>

int myFilter(char *buf, int qt, char *coluna, char *operador, char *operando){

	char* token1 = procura(buf, qt, coluna);
	char* token2 = procura(buf, qt, operando);

    int c1 = atoi(token1);
    int c2 = atoi(token2);

    free(token1);
    free(token2);

	if((strcmp(operador, "=")) == 0){
    	if(c1 == c2) return 1;
    }
    if((strcmp(operador, ">=")) == 0){
    	if(c1 >= c2) return 1;
    }
    if((strcmp(operador, "<=")) == 0){
    	if(c1 <= c2) return 1;
    }
    if((strcmp(operador, ">")) == 0){
    	if(c1 > c2) return 1;
    }
    if((strcmp(operador, "<")) == 0){
    	if(c1 < c2) return 1;
    }
    if((strcmp(operador, "!=")) == 0){
    	if(c1 != c2) return 1;
    }

	return 0;
}

int main(int argc, char** argv){

    if(argc!=4){
        printf("Número de argumentos de filter inválido\n");
        return -1;
    }

	char buf[PIPE_BUF];
	int qt, qtFilter;

	while((qt=readln(0,buf,PIPE_BUF))>0){
        qtFilter = myFilter(buf, qt, argv[1], argv[2], argv[3]);
    	if(qtFilter) write(1, buf, qt);
    	memset(buf, 0, qt);
	}
	
	return 0;
}
