#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include "readln.h"

int node(int id, char* cmd, char** arg, int qtArg){ 

	int x, y, z, nrCharLidos, nrIn, nrOut;

	char comando[PIPE_BUF];
	sprintf(comando, "./%s", cmd);
	
	char** argumentos = (char **) malloc((qtArg+1) * sizeof(char *)); //comando, argumentos do comando
	int i;

	char buffer[PIPE_BUF];

//--------------------------CRIAR PIPE com NOME

	char pipeIn[11];
	sprintf(pipeIn, "/tmp/pipeIn%d",id);

	char pipeOut[11];
	sprintf(pipeOut, "/tmp/pipeOut%d",id);

	x = mkfifo(pipeIn,0666);
	if(x<0){
		printf("erro criar %s\n",pipeIn);
		return -1;
	}

	x = mkfifo(pipeOut, 0666);
	if(x < 0){
		printf("erro criar %s\n",pipeIn);
		return -1;
	}

//--------------------------CRIAR PIPE SEM NOME - "pai escreve"/filho lê conversação

	int pf[2];
	x = pipe(pf);
	if (x<0) {
		printf("erro criar pipe pf\n");
		return -1;
	}

//--------------------------CRIAR PIPE SEM NOME - "pai lê"/filho escreve conversação

	int fp[2];
	x = pipe(fp);
	if (x<0) {
		printf("erro criar pipe fp\n");
		return -1;
	}

//--------------------------CRIAR FILHOS

	x = fork(); // FILHO que lê do pipeIN e escreve no pipe sem nome PF

	if(x==0){ 
		close(pf[0]);
		close(fp[0]);
		close(fp[1]);
		
		nrIn = open(pipeIn, O_RDONLY);
		if(nrIn<0){
			printf("erro open %s\n",pipeIn); //perror
			_exit(-1);
		}
		while((nrCharLidos = readln(nrIn, buffer, PIPE_BUF)) >0){
			write(pf[1], buffer, nrCharLidos);
    		memset(buffer, 0, nrCharLidos);
		}

		close(pf[1]);
		exit(0);
	}
	else {
		y = fork(); // FILHO que le do pipe sem nome PF, executa o comando e escreve no pipe sem nome FP
		if(y==0){ 
			close(pf[1]);
			close(fp[0]);

			argumentos[0]=comando;
			
			for(i=1; i<qtArg+1; i++){
				argumentos[i]=arg[i-1];
			}

			argumentos[qtArg+1]=NULL;

			dup2(pf[0],0);
			dup2(fp[1],1);
			
			close(pf[0]); 
			close(fp[1]);

			execv(comando, argumentos);
			printf("erro exec\n");
			exit(0);
		}
		else{
			z = fork(); // FILHO que lê do pipe sem nome FP e escreve para o pipeOUT
			if(z==0){ 
				close(fp[1]); 
				close(pf[0]); 
				close(pf[1]);

				nrOut = open(pipeOut, O_WRONLY);
				if(nrOut<0){
					printf("erro open %s\n",pipeOut);
					return -1;
				}
				while((nrCharLidos = readln(fp[0], buffer, PIPE_BUF)) >0){
					write(nrOut, buffer, nrCharLidos);
    				memset(buffer, 0, nrCharLidos);
				}

				close(fp[0]);
				exit(0);
			}
			else {
				//pai fecha todos os pipes sem nome
				close(pf[1]);
				close(pf[0]);
				close(fp[1]);
				close(fp[0]);
				wait(NULL);
			}
		}
	}

	free(argumentos);

	return 0;
}

int main(int argc, char** argv){

	if(argc<4){
        printf("Número de argumentos de node inválido\n");
        return -1;
    }

	char** argumentos = (char **) malloc((argc-3) * sizeof(char *));
	int i;

	for(i=3; i<argc; i++){
		argumentos[i-3]=argv[i]; //tira executavel, id, comando
	}

	node(atoi(argv[1]), argv[2], argumentos, argc-3);

	free(argumentos);

	return 0;
}