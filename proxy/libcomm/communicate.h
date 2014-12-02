#ifndef _COMM_H
#define _COMM_H

#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#define NONE (fd_set *) NULL

/* Fonction associee au role de simple proxy. Ecoute sur la socket du   */
/* cote client (fd) et sur la socket du cote serveur (rfd) et transmet  */
/* (en utilisant le buffer), dans le sens approprie, ce qui se presente */
void communicate(int fd, int rfd, char buffer[4096]);

#endif 
