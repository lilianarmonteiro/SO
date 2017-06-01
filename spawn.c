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

	int r, x, i, a, j, status = 0, qtConst = 0, coluna=1, flag = 0;
	char str[256];


	printf("NÚMERO DE ARGS: %d\n", nrArgs);
	for(i=0; i <nrArgs; i++){
		printf("ARGUMENTO %d: %s\n", i, args[i]);
	}
	

	x = fork();

	if(x == 0){
		for(i = 0; i < nrArgs && args[i] != '\0' && flag == 0; i++){
			//printf("entreeeeeei %d vezes\n",i );
			if(args[i][0] == 'a'){
				a = atoi(&args[i][1]);

				printf("COLUNA: %d\n",a);

				for(j = 0; buf[j] != '\n' && flag == 0; j++){
					if(buf[j] == ':') coluna++;
					if(coluna == a){
						args[i] = &buf[coluna];
						flag = 1;
					}
				}
				
				for(i=0; i < nrArgs; i++)
					printf("ARGUMENTO %d: %s\n",i, args[i]);
			}
		}
		//execv(cmd, args);
		exit(-1);
	}

	else{
		wait(&r);//, &status, WUNTRACED);

		sprintf(str, "%d", WEXITSTATUS(r));
		qtConst = myConst(buf, qt, str);
		return qtConst;
	}

	return 0;


}

int main(int argc, char** argv){
	char buf[PIPE_BUF];
	char** argumentos = (char **) malloc((argc-1) * sizeof(char *));
	int i;
	int qt, qtSpawn;

	if(argc < 3){
		printf("Número de argumentos de spawn inválido\n");
        return -1;
	}

	

	for(i=2; i<argc; i++){
		argumentos[i-2]=argv[i]; //tira executavel + o comando
	}

	while((qt=readln(0,buf,PIPE_BUF))>0){
		qtSpawn = mySpawn(buf, qt, argv[1], argumentos, argc-2);
    	write(1, buf, qtSpawn);
	}
	
	return 0;

}