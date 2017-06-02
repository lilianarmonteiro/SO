CC = gcc
CFLAGS = -Wall 

#disconnect.o
controlador: connect.o node.o inject.o readln.o procura.o
	$(CC) $(CFLAGS) controlador.c -o controlador connect.o node.o inject.o readln.o procura.o

readln.o:
	$(CC) $(CFLAGS) -c readln.c

addFimLinha.o:
	$(CC) $(CFLAGS) -c addFimLinha.c

procura.o:
	$(CC) $(CFLAGS) -c procura.c

connect.o: readln.o
	$(CC) $(CFLAGS) -c connect.c readln.o

#disconnect.o:
#	$(CC) $(CFLAGS) -c disconnect.c

node.o: readln.o
	$(CC) $(CFLAGS) -c node.c readln.o

inject.o:
	$(CC) $(CFLAGS) -c inject.c

const: readln.o addFimLinha.o
	$(CC) $(CFLAGS) const.c -o const readln.o addFimLinha.o

filter: readln.o procura.o
	$(CC) $(CFLAGS) filter.c -o filter readln.o procura.o

window: readln.o addFimLinha.o procura.o
	$(CC) $(CFLAGS) window.c -o window readln.o addFimLinha.o procura.o

spawn: readln.o addFimLinha.o procura.o
	$(CC) $(CFLAGS) spawn.c -o spawn readln.o addFimLinha.o procura.o

clean:
	rm -f *.o controlador const filter window spawn