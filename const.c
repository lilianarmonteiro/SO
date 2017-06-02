#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "readln.h"
#include "addFimLinha.h"

#include <stdio.h>

int main(int argc, char** argv){

	if(argc!=2){
        printf("Número de argumentos de const inválido\n");
        return -1;
    }

	char buf[PIPE_BUF];
	int qt, qtConst;

	while((qt=readln(0,buf,PIPE_BUF))>0){
		qtConst = addFimLinha(buf, qt, argv[1]);
    	write(1, buf, qtConst);
    	memset(buf, 0, qtConst);
	}
	
	return 0;
}

