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

connect.o: 
	$(CC) $(CFLAGS) -c connect.c readln.c

#disconnect.o:
#	$(CC) $(CFLAGS) -c disconnect.c

node.o:
	$(CC) $(CFLAGS) -c node.c readln.c

inject.o:
	$(CC) $(CFLAGS) -c inject.c

const: readln.o addFimLinha.o
	$(CC) $(CFLAGS) const.c readln.o addFimLinha.o -o const

filter: readln.o procura.o
	$(CC) $(CFLAGS) filter.c readln.o procura.o -o filter

window: readln.o addFimLinha.o procura.o
	$(CC) $(CFLAGS) window.c readln.o addFimLinha.o procura.o -o window

spawn: readln.o addFimLinha.o procura.o
	$(CC) $(CFLAGS) spawn.c readln.o addFimLinha.o procura.o -o spawn

clean:
	rm -f *.o controlador const filter window spawn