#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "procura.h"

char* procura(char* buf, int qt, char *coluna){
	char aux[qt];
	strcpy(aux,buf);

    const char *s = ":\n";
    char *token;

    /* get the first token */
    token = strtok(aux, s);

    int i=1;
    while( i<atoi(coluna) ) 
    {
       token = strtok(NULL, s);
       i++;
    }

    char* aux2 = malloc(strlen(token)*sizeof(char));
    strcpy(aux2,token);

   	return aux2;
}
