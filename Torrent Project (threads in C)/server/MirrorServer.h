#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h> // open
#include <stdlib.h>
#include <string.h>
#include <signal.h> // signal
#include <unistd.h> // fork
#include <pthread.h> // threads
#include <sys/stat.h> // mkdir

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

#define true 1
#define false 0
#define QUEUE_SIZE 5

/* Global variables */
int end;
int queue_size;
int numDevices;

char* path;

pthread_mutex_t lock;

pthread_cond_t cond_full;
pthread_cond_t cond_empty;
pthread_mutex_t cond_lock;
/* ----------------- */

/* queque stracture */
typedef struct
{
	int rear;
    int front;
    char **elements;
}Queue;

Queue* Q;

typedef struct
{
	int ContentPort;
	char* ContentAdrress;
}Connect;

Connect fut_con[10];

/* Declarations of functions */

/* server_assistant.c */
void server_assistant(int );

void terminator(int );

/* functions.c */
void free_table(char** );
void show_error(char* , int );
int isDirectory(const char *);
int dir_exists(char** , char* );

int ip_check(char* );
int write_all(int , char* , int );

char** analyze(char* , char* , int );

/* thread.c */
void *thread_start(void* );

/* Worker.c */
void *thread_worker(void* );

/* queque.c */
Queue* queue_init();

void Dequeue(Queue* );
void free_queue(Queue* );
void Enqueue(Queue* , char* );

char* front(Queue* );
