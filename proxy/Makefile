CC=gcc
CFLAGS=-Wall -Werror -Wextra

all : proxy

proxy : proxy.o proxy_fonctions.o
	$(CC) $(CFLAGS) proxy.o proxy_fonctions.o libcomm/libcommunicate.a -o proxy

proxy.o : proxy.c proxy.h proxy_fonctions.h
	$(CC) $(CFLAGS) -c proxy.c

proxy_fonctions.o : proxy_fonctions.c proxy.h proxy_fonctions.h
	$(CC) $(CFLAGS) -c proxy_fonctions.c

clean :
	rm -rf *~ *.o

mrproper :
	make clean
	rm all
