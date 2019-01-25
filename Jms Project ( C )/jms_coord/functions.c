#include "main.h"

/* disconnect the fifo in order to create again */
void disconnect(char* fifo_in, char* fifo_out)
{
	unlink(fifo_in);
	unlink(fifo_out);
}

/* Check if any job has terminate */
int check_kids(jms_job job_table[], int max_jobs, int count)
{
	pid_t pid;
	int j, status;
	
	while ( (pid = waitpid(-1,&status,WNOHANG)) > 0 ) 
	{	
		for(j = 1; j <= max_jobs; j++)
		{
			if( pid == job_table[j].job_id )
			{
				printf("Pool: job_%d terminate successfully.\n",job_table[j].job_num);
				strcpy(job_table[j].status,"Finished");
				break;	
			}
		}	
		count++;
	}
	return count;
}

/* Rearange the str because we dont want the command (e.g submit) to the table */
char** rearrange(char** buffer)
{	
	int i = 0, j = 1;
	
	char** final = (char**)malloc(sizeof(char*)*10);
	if ( final == NULL ) { perror("Failed to malloc"); exit(10); }

	while( buffer[j] != NULL )
	{	
		final[i] = (char*)malloc(sizeof(char)*strlen(buffer[j])+1);
		if( final[i] == NULL ) { perror("Failed to malloc"); exit(22); }

		strcpy(final[i],buffer[j]);
		i++;
		j++;
	}
	final[i] = NULL;
	
	return final;
}

/* Split the string into tokens */
char** analyze(char* buffer)
{
	int i= 0;
	char hold[100];
	strcpy(hold,buffer);
	
	char* token;
	
	char** final = (char**)malloc(sizeof(char*)*10);
	if ( final == NULL ) { perror("Failed to malloc"); exit(10); }
	
	token = strtok(hold, " ");
	while( token != NULL )
	{
		final[i] = (char*)malloc(sizeof(char)*strlen(token)+1);
		if( final[i] == NULL ) { perror("Failed to malloc"); exit(11); }
		
		strcpy(final[i],token);
		
		i++;
		token = strtok(NULL, " ");
	}
	final[i] = NULL;

	return final;
}


/* stick an str behind another */
char* concat(const char *s1, const char *s2)
{
    char* final = (char*)malloc(strlen(s1)+strlen(s2)+1);
    if( final == NULL )
    {
    	perror("Failed to malloc:");
    	exit(11);
    }
    
    strcpy(final, s1);
    strcat(final, s2);
    return final;
}

void init_struct(jms_job job_table[], int max_jobs)
{
	int i;
	
	for(i = 1; i <= max_jobs; i++)
	{
		strcpy(job_table[i].status,"NULL");
	}
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
