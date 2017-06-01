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

int mySpawn(char *buf, int qt, char *cmd, char **args, int nrArgs){
	int status, qtConst, x, r, i, k, m, n;
	char str[256], tmp[256], c1[qt];
	int linhas[nrArgs];
	char argumentos[nrArgs];

	for(i=0; i<nrArgs; i++){
		linhas[i] = -1;
	}

	for(i=0; i<nrArgs; i++){
		tmp = args[i];
		if(tmp[0] == '$') linhas[i] = atoi(tmp[1]);
		else argumentos[i] = tmp;
	}

	for(i=0; i<nrArgs; i++){
		memset(c1,'\0',qt);
		if(linhas[i] != -1){
			for(k=1, m=0; k<linhas[i]; k++, m++){
    			while(buf[m] != ':') m++;
    		}
    		n=m;
    		while(buf[m] != ':' && buf[n] != '\n') m++;
    		strncpy(c1, &buf[n], m-n);
    		argumentos[i] = c1;
		}
	}

	x = fork();

	if(x==0){
		execv(cmd, argumentos);
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
		qtSpawn = mySpawn(buf, qt, argc[1], &argc[2], argv-2);
    	write(1, buf, qtSpawn);
	}
	
	return 0;
}