#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include "readln.h"

int myConst(char *buf, int qt, char* value){
    int size = strlen(value);

    buf[qt-1] = ':';
    strcpy(&buf[qt], value);
    qt = qt+size;
    buf[qt] = '\n';

	return qt+1;
}

int mySpawn(char *buf, int qt, char *cmd, char **args){
	int status, qtConst, x, r;
	char str[256];

	x = fork();

	if(x==0){
		//exec...
	}

	else{
		wait(&r);
		status = WEXITSTATUS(r);
		sprintf(str, "%d", status);
		qtConst = myConst(buf, qt, str);
		return qtConst;
	}

	return 0;
}

int main(int argv, char** argc){
	char buf[PIPE_BUF];
	int qt, qtSpawn;

	if(argv < 3){
		printf("Número de argumentos de spawn inválido\n");
        return -1;
	}

	while((qt=readln(0,buf,PIPE_BUF))>0){
		qtSpawn = mySpawn(buf, qt, argc[1], &argc[2]);
    	write(1, buf, qtSpawn);
	}
	
	return 0;
}