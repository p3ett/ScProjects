#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h> // open
#include <sys/stat.h> // isDirectory
#include <signal.h> // signal
#include <unistd.h> // fork
#include <pthread.h> // threads

#include <netdb.h> // gethostbyaddr, struct hostent
#include <sys/wait.h> // waitpid
#include <sys/types.h> // socket
#include <arpa/inet.h> // inet_ntoa(struct in_addr in);
#include <sys/socket.h> // socket
#include <netinet/in.h> // socket, struct in_addr, htonl

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

/* Union semun */
union semun 
{
	int val; 				/* value for SETVAL */
	struct semid_ds *buf;   /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array;  /* array for GETALL, SETALL */
};

#define SHMKEY (key_t)4321
#define SEMKEY (key_t)9876

#define true 1
#define false 0

#define ch_size 100

typedef struct
{
	int id;
	int delay;
}contentServer;

/* Declarations of functions */

/* server_assistant.c */
void server_assistant(int, char* , int , int , int ,int* ,int* , int* , contentServer* );

/* functions.c */
void sanitize(char* );
void free_table(char** );
void make_list(int , char* , char* );

void schild_handler(int sig);

int sem_P(int );
int sem_V(int );
int sem_Init(int , int );
void free_resources(int , int ) ;

int isDirectory(const char* );
int write_all(int , char* , int );

char** analyze(char* , char* , int );
