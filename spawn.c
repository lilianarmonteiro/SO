#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include "readln.h"
#include "addFimLinha.h"
#include "procura.h"

int mySpawn(char *buf, int qt, char *cmd, char **args, int nrArgs){

	int r, son, i, a, qtConst = 0;
	char str[5];

	for(i = 1; i < nrArgs; i++){
		if(args[i][0] == '$') args[i] = procura(buf,qt,&args[i][1]);
	}

	args[nrArgs]=NULL;

	son = fork();

	if(son == 0){
		execvp(cmd, args);
		exit(-1);
	}
	else{
		wait(&r);

		sprintf(str, "%d", WEXITSTATUS(r));
		qtConst = addFimLinha(buf, qt, str);
		return qtConst;
	}

	return 0;
}

int main(int argc, char** argv){
	char buf[PIPE_BUF];
	char** argumentos = (char **) malloc((argc-1) * sizeof(char *));
	int i, qt, qtSpawn;

	if(argc < 3){
		printf("Número de argumentos de spawn inválido\n");
        return -1;
	}

	for(i=1; i<argc; i++){
		argumentos[i-1]=argv[i]; //tira executavel
	}

	while((qt=readln(0,buf,PIPE_BUF))>0){
		qtSpawn = mySpawn(buf, qt, argv[1], argumentos, argc-1);
    	write(1, buf, qtSpawn);
	}
	
	return 0;
}
