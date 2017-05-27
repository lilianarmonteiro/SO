#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include "readln.h"

#include <stdio.h>

int conta_palavras(char* texto){
	int palavras=0;
	int i=0, marca=0; //para o caso de mais que um espaçamento no fim "word  "

	while(texto[i] != '\n'){
    	while(texto[i]==' ' && texto[i]!='\n'){
    		i++;
    		marca=1;
    	}
    	while(texto[i]!=' ' && texto[i]!='\n'){
    		i++;
    		marca=0;
   		}
   		if (marca==0) palavras++;
   		if (texto[i] == '\n') return palavras;
   		else i++; //avança o espaço encontrado e vai para '\n', ou vai para nova palavra, ou novo espaçamento
    }

	return palavras;
}

char** buscarArg(char* buf){
	int i, qtArg = conta_palavras(buf);
	char** Arg = (char **) malloc((qtArg) * sizeof(char *));
	for(i=0; i<qtArg; i++) Arg[i] = malloc(PIPE_BUF * sizeof(char));

	char aux[PIPE_BUF];
	strcpy(aux,buf);

    const char *s = " ";
    char *token;

    /* get the first token */
    token = strtok(aux, s);
    sprintf(Arg[0], "./%s",token);

    i=1;
    while( token!=NULL ) 
    {
        token = strtok(NULL, s);
        Arg[i] = token;
        i++;
    }

    return(Arg);
}

int cria(char* buf){
	int x;
	char** Arg = buscarArg(buf);

	x = fork();
	if(x==0){
		execv(Arg[0], Arg);
		printf("erro exec %s no controlador\n",Arg[0]);
		exit(0);
	}
	else{
		wait(NULL);
	}

	return 0;
}

int main(){
	char buffer[PIPE_BUF];
	char cmd[10];
	int charLidos, i=0;

	int filde = open("controlador.txt", O_RDONLY, 0666);
	if(filde<0){
		printf("erro open controlador.txt\n");
		return -1;
	}

	while((charLidos = readln(filde, buffer, PIPE_BUF)) > 0){
		cria(buffer); //node, connect
		memset(buffer,0,charLidos);
	}

	int x = mkfifo("/tmp/pipeControlador",0666);
	if(x<0){
		printf("erro criar /tmp/pipeControlador\n");
		return -1;
	}

	int pipe = open("/tmp/pipeControlador", O_RDONLY);
	if(pipe<0){
		printf("erro open /tmp/pipeControlador\n");
		return -1;
	}
	while((charLidos = readln(pipe, buffer, PIPE_BUF)) >0){
		cria(buffer); //outros comandos passados dps de definida a rede, ex: inject, disconnect
		memset(buffer,0,charLidos);
	}

	return 0;
}