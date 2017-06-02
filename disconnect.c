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

int myDisconnect(char** argumentos, int qtArgs, int pid){
	int x;
	x = fork();

	if( x == 0){
		kill(pid,SIGTERM);
	}
	else{
		wait(NULL);
	}
}

int main(int argc, char** argv){

	if(argc < 3){
        printf("Número de argumentos de connect inválido\n");
        return -1;
    }

	char** argumentos = (char **) malloc((argc-1) * sizeof(char *));
	int i;

	for(i=1; i<argc; i++){
		argumentos[i-1]=argv[i]; //tira executavel
	}

	pid=connect();

	myDisconnect(argumentos, argc-1, pid);
}