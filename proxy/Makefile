CC=gcc
CFLAGS=-Wall -Werror -Wextra

all : proxy
	
proxy : proxy.o
	$(CC) $(CFLAGS) proxy.o libcomm/libcommunicate.a -o proxy

proxy.o : proxy.c proxy.h
	$(CC) $(CFLAGS) -c proxy.c
	
client_fictif : client_fictif.o
	$(CC) $(CFLAGS) client_fictif.o -o client_fictif

client_fictif.o : client_fictif.c client_fictif.h
	$(CC) $(CFLAGS) -c client_fictif.c

clean :
	rm -rf *~ *.o

mrproper :
	make clean
	rm all
