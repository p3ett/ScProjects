#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>

#define true 1
#define false 0
#define size 10

typedef struct
{
	int job_num;
	int job_id;
	char status[10];
}jms_job;

/* Decleration of functions*/

/* functions.c */
void disconnect(char* , char* );
void free_table(char** );

char** analyze(char* );
char** rearrange(char** );
char* concat(const char* , const char* );

void init_struct(jms_job* , int );
int check_kids(jms_job* , int , int );

/* job.c */
void exec_job(char** , char* , int  );

/* pool.c */
void pool_process(char* , int , int , int );

/* Commands */
int status(char* , int , int , int );
void status_all(char* , int , int , int );

void showActive(char* , int , int );
void showFinished(char* , int , int , int );
void showPools(char* , int , int ,pid_t* pool);
int suspend(char* , int , int , int );
int resume(char* , int , int , int );
void shutdown(pid_t* pool, char* , int , int , int );

