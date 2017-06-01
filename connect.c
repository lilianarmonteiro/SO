#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include "readln.h"

int fanout(char** argumentos, int qtArg){
	int i, x,a;
	char buffer[PIPE_BUF];
	int f[qtArg];
	int c;
	
	char pipeOut[11];	
	sprintf(pipeOut, "/tmp/pipeOut%s",argumentos[0]);

	f[0] = open(pipeOut, O_RDONLY, 0666);
	
	char pipeIn[i][11];
	for(i = 1; i < qtArg-1; i++){
		sprintf(pipeIn[i], "/tmp/pipeIn%s", argumentos[i]);
		f[i] = open(pipeIn, O_WRONLY,0666);
	}

	//FALTA MUDAR PARA "PIPE IN %d" DOS VARIOS ARGUMENTOS[i]

	for(i=1; i < qtArg; i++){
		f[i] = open(argumentos[i], O_WRONLY, 0666);
	}
	
	while((c = readln(f[0], buffer, PIPE_BUF))>0){
		for(i=1; i < qtArg; i++){
			write(f[i],buffer,c);
		}
		memset(buffer,0,c);
	}
	
	return 0;
}

int main(int argc, char** argv){

	if(argc<3){
        printf("Número de argumentos de connect inválido\n");
        return -1;
    }

	char** argumentos = (char **) malloc((argc-1) * sizeof(char *));
	int i;

	for(i=1; i<argc; i++){
		argumentos[i-1]=argv[i]; //tira executavel
	}

	fanout(argumentos, argc-1);

	free(argumentos);

	return 0;
}