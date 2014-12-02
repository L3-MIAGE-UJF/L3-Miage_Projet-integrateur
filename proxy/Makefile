CC=gcc
CFLAGS=-Wall -Werror -Wextra

all : proxy
	
proxy : proxy.o
	$(CC) $(CFLAGS) proxy.o -o proxy

proxy.o : proxy.c proxy.h
	$(CC) $(CFLAGS) -c proxy.c
	
clean :
	rm -rf *~ *.o

mrproper :
	make clean
	rm proxy
