#include <unistd.h>
#include <stdlib.h>


int main() {
    int pd[2], x ;
    char *buf;
    char *a = "$1";
    char* command = "./a.out";
    pipe(pd);

    if(!fork()) {
        close(pd[1]);
        dup2(pd[0],0);
        execlp(command,command,a,NULL);
        close(pd[0]);
        _exit(0);
    }
    else {
        close(pd[0]);
        while((x = read(0,&buf,100))> 0)
            write(pd[1],&buf,x);
        close(pd[1]);
        _exit(0);
    }

    return 0;
}
