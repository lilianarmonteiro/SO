#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include <stdio.h>

int addFimLinha(char *buf, int qt, char* value){
    int size = strlen(value);

    buf[qt-1] = ':';
    strcpy(&buf[qt], value);
    qt = qt+size;
    buf[qt] = '\n';

	return qt+1;
}