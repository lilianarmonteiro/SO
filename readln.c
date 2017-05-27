#include <stdlib.h>
#include <unistd.h>
#include "readln.h"

//#include <limits.h>

int readln(int fd, char *buffer, int sizeBuffer){
	int i=0, r=0;
	char c;

	do{
		r = read(fd, &c, 1);
		if(r>0) buffer[i++] = c;
	}while(i<sizeBuffer && r>0 && c != '\n');

	return i;
}

/*int main(){
	char buf[PIPE_BUF];
	int i, qt;

	while((qt=readln(0,buf,PIPE_BUF))>0){
    	write(1, buf, qt);
	}
	
	return 0;
}*/
