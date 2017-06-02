#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include "readln.h"
#include "node.h"
#include "connect.h"
#include "procura.h"
#include "inject.h"

#include <stdio.h>

int mainInject(int argc, char** argv){

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

	return 0;
}


int mainConnect(int argc, char** argv){
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

	return pid;
}


int mainNode(int argc, char** argv){

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

	return 0;
}

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
	char** Arg = (char **) malloc((qtArg+1) * sizeof(char *));
	for(i=0; i<qtArg+1; i++) Arg[i] = (char*) malloc(PIPE_BUF * sizeof(char));

	char* aux = malloc(PIPE_BUF*sizeof(char));
	strcpy(aux,buf);

    const char *s = " \n";
    char *token;

    /* get the first token */
    token = strtok(aux, s);

    i=0;
    while( token!=NULL ) 
    {
        strcpy(Arg[i],token);
        token = strtok(NULL, s);
        i++;
    }

    free(aux);
    Arg[i]=NULL;

    return(Arg);
}

int cria(char* buffer, int charLidos, int* matriz[60], int* array){
	int argc, pid, dis=0, posI=0, pos, i, linha=-1, coluna=-1;
	char** Arg = buscarArg(buffer);

	for(argc=0;Arg[argc]!=NULL;argc++);

	for(i=0; i<60; i++) array[i]=-1;

	if(strcmp(Arg[0],"node")==0) mainNode(argc,Arg);
	else if (strcmp(Arg[0],"connect")==0) {
		linha=atoi(Arg[1]);
		for(coluna=0;coluna<60;coluna++){ //ver se já à connects nesse nodo
			if(matriz[linha][coluna]!=-1){
				array[posI]=coluna;
				posI++;
				dis=1;
				pid=matriz[linha][coluna];
			}
		}

		if(dis==0) { //se nao haver connects, faz e atualiza matriz
			pid = mainConnect(argc,Arg);
			for(i=2;Arg[i]!=NULL;i++) matriz[linha][atoi(Arg[i])]=pid;
		}
	    else { //se ja haver connects, faz disconnect, connect dos antigos e dos novos, e atualiza matriz
			int tam = posI+argc;
			char** argC = (char **) malloc(tam * sizeof(char *));
			for(i=0; i<posI+argc; i++) argC[i] = malloc(PIPE_BUF * sizeof(char));
		
			pos=posI;

			sprintf(argC[0],"%d",linha); //node principal
			pos++;

			for(i=1;i<pos;i++){
				sprintf(argC[i],"%d",array[i-1]); //nodos antigos
			}
			
			//mainDisconnect(pos, argC, pid); //disconnect antigos
			
			for(i=2;i<argc;i++){
				if(matriz[linha][atoi(Arg[i])]==-1) strcpy(argC[pos++],Arg[i]); //add novos se nao estao nos antigos
			}

			pid = mainConnect(pos, argC); //faz connect
			
			for(i=1; i<pos;i++) matriz[linha][atoi(argC[i])]=pid; //atualiza matriz

			for(i=0; i<posI+argc; i++){
				free(argC[i]);
			}
			free(argC);
		}
	}
	/*else if (strcmp(Arg[0],"disconnect")==0) {
		linha=atoi(Arg[1]);
		for(coluna=0;coluna<60;coluna++){ //ver se já à connects nesse nodo
			if(matriz[linha][coluna]!=-1){
				array[pos]=coluna;
				pos++;
				dis=1;
				pid=matriz[linha][coluna];
			}
		}
		if(dis==0) return -1;
		else{
			char** argC = (char **) malloc((pos+argc) * sizeof(char *));
			for(i=0; i<pos; i++) argC[i] = malloc(2 * sizeof(char));

			sprintf(argC[0],"%d",linha);
			pos++;

			for(i=1;i<pos;i++){
				sprintf(argC[i],"%d",array[i-1]);
			}

			//mainDisconnect(pos, argC, pid);
			
			for(i=1;i<pos;i++){
				sprintf(argC[i],"%d",array[i-1]);
			}

			for(i=1;i<argc;i++){
				if(matriz[linha][atoi(Arg[i])]==-1) strcpy(argC[pos++],Arg[i]);
			}

			pid = mainConnect(pos, argC);
			for(i=1; i<pos;i++){
				matriz[linha][atoi(Arg[i])]=pid;
				matriz[atoi(Arg[i])][linha]=pid;
			}
		}
	}*/
	else if (strcmp(Arg[0],"inject")==0) mainInject(argc,Arg);

	return 0;
}

int main(){
	char* buffer = malloc(PIPE_BUF*sizeof(char));
	int charLidos, fildeCont, i, j;
	int* array = malloc(60*sizeof(int)); //array que vai conter connects antigos
	int** matriz=NULL; //312 forks maximo, 3 node, 1 connect
	matriz = malloc(60 * sizeof(int*));

	for(i=0; i <60;i++) {
		matriz[i] = (int*) malloc(60* sizeof(int));
		for(j=0; j<60;j++){
			matriz[i][j] = -1;
		}
	}

	fildeCont = open("controlador1.txt", O_RDONLY, 0666); //MUDARRRRRRRRRRRRRRRRRRRRRRRRRRRRrr
	if(fildeCont<0){
		printf("erro open controlador.txt\n");
		return -1;
	}

	//cria a REDE predefinida
	while((charLidos = readln(fildeCont, buffer, PIPE_BUF)) > 0){
		write(1,buffer,charLidos); //APAGAR DPSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
		cria(buffer,charLidos,matriz,array);
		memset(buffer,0,charLidos);
	}
	
	//edita a REDE criada
	while((charLidos = readln(0, buffer, PIPE_BUF)) >0){
		write(1,buffer,charLidos); //APAGAR DPSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
		cria(buffer,charLidos,matriz,array); //outros comandos passados dps de definida a rede, ex: inject, disconnect
		memset(buffer,0,charLidos);
	}
	
	return 0;
}
