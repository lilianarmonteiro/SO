#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int inject(char* id, char* cmd, char** arg, int qtArg){ 

	int y;
	
	char** argumentos = (char **) malloc((qtArg+1) * sizeof(char *)); //comando, argumentos do comando
	int i;

	char pipeIn[11];
	sprintf(pipeIn, "/tmp/pipeIn%s",id);
	int in;

	y = fork(); // FILHO que excuta o comando, injetando no node id o que estiver no txt
	if(y==0){ 
		argumentos[0]=cmd;

		for(i=1; i<qtArg+1; i++){
			argumentos[i]=arg[i-1];
		}

		argumentos[qtArg+1]=NULL;

		in = open(pipeIn, O_WRONLY);
		if(in<0){
			printf("erro open %s\n",pipeIn);
			return -1;
		}

		dup2(in,1);
			
		execvp(cmd, argumentos); //cat input.txt , lê do ficheiro e escreve para o node
		printf("erro exec\n");
		exit(0);
	}
	else{
		wait(NULL);
	}
	
	free(argumentos);

	return 0;
}

int main(int argc, char** argv){

	if(argc<4){
        printf("Número de argumentos de inject inválido\n");
        return -1;
    }

	char** argumentos = (char **) malloc((argc-3) * sizeof(char *));
	int i;

	for(i=3; i<argc; i++){
		argumentos[i-3]=argv[i]; //tira executavel, id, comando
	}

	inject(argv[1], argv[2], argumentos, argc-3);

	free(argumentos);

	return 0;
}