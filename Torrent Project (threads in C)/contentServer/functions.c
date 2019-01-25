#include "ContentServer.h"

int write_all(int sock, char* buffer, int size)
{
	int nwrite, sent = 0;
	
	while( sent < size )
	{
		if( (nwrite = write(sock, buffer + sent, size - sent)) < 0 ) 
		{ perror("Write"); exit(1); }
	
		sent += nwrite;
	}
	
	return sent;
}

void make_list(int newsock, char* buffer, char* path)
{
	int i = 0, size, dir_or_not;
	
	char ch;
	char hold[256];
	char hold2[256];
	
	FILE *pipe_fp; /* Use popen to run ls */
		
	/* Invoke ls through popen */
	if( (pipe_fp = popen(buffer, "r")) == NULL )
	{ perror("Popen"); exit(1); }
		
	/* Transfer data from ls to MirrorManager */
	while( (ch = getc(pipe_fp)) != EOF )
	{		
		if( ch == '\n' )
		{
			buffer[i] = '\0';
		
			/* Check if buffer is a directory */
			sprintf(hold, "%s/%s",path,buffer);
			
			if( isDirectory(hold) == true )
			{
				/* Send to MirrorManager if you send a dir or file */	
				dir_or_not = 1;
				if( write(newsock, &dir_or_not, sizeof(dir_or_not)) < 0 ) { perror("Write"); exit(1); }
				
				/* Sent the size of data that server has to read */	
				size = strlen(hold)+1;
				if( write(newsock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }

				/* write all the data to MirrorManager */
				write_all(newsock, hold, strlen(hold)+1);
				
				snprintf(hold2, sizeof(hold), "ls %c%s/%s%c",'"',path,buffer,'"');
					
				make_list(newsock, hold2, hold);
			}
			else
			{
				/* Send to MirrorManager if you send a dir or file */
				dir_or_not = 0;
				if( write(newsock, &dir_or_not, sizeof(dir_or_not)) < 0 ) { perror("Write"); exit(1); }
				
				/* Sent the size of data that server has to read */	
				size = strlen(hold)+1;
				if( write(newsock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }

				/* write all the data to MirrorManager */
				write_all(newsock, hold, strlen(hold)+1);			
			}
				
			i = 0;
			continue;
		}
		
		buffer[i] = ch;
		i++;
	}
	
	pclose(pipe_fp);
}

void sanitize(char* str)
{
	char *src, *dest;
	
	for(src = dest = str; *src; src++)
		if( * src == '/' || isalnum (* src ) )
			*dest++ = *src;
		
	*dest = '\0';
}

int isDirectory(const char *path) 
{
   struct stat statbuf;

   if( stat(path, &statbuf) != 0 )
   { perror("Stat"); return -1; }

   return S_ISDIR(statbuf.st_mode);
}

/* Split the string into tokens */
char** analyze(char* buffer, char* delimiter, int size)
{
	int i= 0;
	char hold[size];

	strcpy(hold,buffer);
	
	char* token;
	
	char** final = (char**)malloc(sizeof(char*)*5);
	if ( final == NULL ) { perror("Failed to malloc"); exit(1); }
	
	token = strtok(hold, delimiter);
	while( token != NULL )
	{
		final[i] = (char*)malloc(sizeof(char)*strlen(token)+1);
		if( final[i] == NULL ) { perror("Failed to malloc"); exit(1); }
		
		strcpy(final[i],token);
		
		i++;
		token = strtok(NULL, delimiter);
	}
	final[i] = NULL;

	return final;
}

void free_table(char** table)
{
	int i = 0;

	while( table[i] != NULL )
	{
		free(table[i]);
    	i++;
  	}

	free(table);
}

void show_error(char* str, int err)
{
	fprintf(stderr, "%s: %s\n",str, strerror(err));
	exit(1);
}

void free_resources(int shm_id, int sem_id) 
{
	/* Delete the shared memory segment */
	if( shm_id != 0 )
		shmctl(shm_id, IPC_RMID, NULL);

	/* Delete the semaphore */
	if( sem_id != 0 )
		semctl(sem_id, 0, IPC_RMID, 0);
}

/* Semaphore P - down operation , using semop */
int sem_P(int sem_id)
{ 
	struct sembuf sem_d;

	sem_d.sem_num = 0;
	sem_d.sem_op = -1;
	sem_d.sem_flg = 0;

	if( semop(sem_id , & sem_d, 1) == -1 ) 
	{ perror("# Semaphore down (P) operation.\n"); return -1; }

	return 0;
}

/* Semaphore V - up operation, using semop */
int sem_V(int sem_id)
{
	struct sembuf sem_d ;

	sem_d.sem_num = 0;
	sem_d.sem_op = 1;
	sem_d.sem_flg = 0;

	if( semop(sem_id, &sem_d, 1) == -1 ) 
	{ perror("# Semaphore up (V) operation.\n"); return -1; }

	return 0;
}

/* Semaphore Init - set a semaphore’s value to val */
int sem_Init(int sem_id, int val)
{
	union semun arg;
	
	arg.val = val;

	if( semctl(sem_id, 0, SETVAL, arg) == -1 ) 
	{ perror("# Semaphore setting value.\n"); return -1; }
	
	return 0;
}

/* Wait for all dead child processes */
void schild_handler(int sig)
{	
	while( waitpid(-1, NULL, WNOHANG) > 0 );
}
