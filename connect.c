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
#include "connect.h"

int myConnect(char** argumentos, int qtArg){
	int i, x, pid = 0;
	char buffer[PIPE_BUF];
	int f[qtArg];
	int c;

	//--------------ABRE O PIPE OUT PARA LER E OS PIPES INS PARA ESCRITA-------------------------

	char pipeOut[14];	
	sprintf(pipeOut, "/tmp/pipeOut%s",argumentos[0]);

	f[0] = open(pipeOut, O_RDONLY, 0666);
	if(f[0] < 0) {
		printf("erro open %s\n",pipeOut);
		return -1;
	}

	char pipeIn[14];
	for(i = 1; i < qtArg ; i++){
		sprintf(pipeIn, "/tmp/pipeIn%s", argumentos[i]);
		f[i] = open(pipeIn, O_WRONLY, 0666);
		if(f[i] < 0) {
			printf("erro open %s",pipeIn);
			return -1;
		}
		memset(pipeIn,0,14);
	}

	//-------------------- LÊ DO PIPE OUT, O FILHO ESCREVE NOS PIPEINS--------------------------
	
	x = fork();

	if(x==0){
		while((c = readln(f[0], buffer, PIPE_BUF))>0){
			for(i = 1; i < qtArg ; i++){
				write(f[i],buffer,c);
			}
			memset(buffer,0,c);
		}
		exit(0);
	}
	else{
		pid = getpid();
		return pid;
	}

	return 0;
}

/*int main(int argc, char** argv){

	int pid = 0;

	if(argc<3){
        printf("Número de argumentos de connect inválido\n");
        return -1;
    }

	char** argumentos = (char **) malloc((argc-1) * sizeof(char *));
	int i;

	for(i=1; i<argc; i++){
		argumentos[i-1]=argv[i]; //tira executavel
	}

	pid = myConnect(argumentos, argc-1);

	free(argumentos);

	return pid;
}*/