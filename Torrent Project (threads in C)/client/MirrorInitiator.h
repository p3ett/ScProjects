#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> // signal
#include <unistd.h> // fork

#include <netdb.h> // gethostbyaddr, struct hostent
#include <sys/wait.h> // waitpid
#include <sys/types.h> // socket
#include <arpa/inet.h> // inet_ntoa(struct in_addr in);
#include <sys/socket.h> // socket
#include <netinet/in.h> // socket, struct in_addr, htonl

#define true 1
#define false 0

int content_num;

/* Declarations of functions */

/* functions.c */
void free_table(char** );

int ip_check(char* );
int write_all(int , char* , int );

char** analyze(char* , char* , int );

/* client_assistant.c */
void client_assistant(int , int , char** );
