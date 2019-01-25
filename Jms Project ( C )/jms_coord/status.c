#include "main.h"

int status(char* path, int fd_out, int jobid, int max_jobs)
{
	int i, j = 0;
	int max = max_jobs;
	int p_in, p_out, nread, nwrite;
	
	char buffer[100];
	char pool_in[100];
	char pool_out[100];
	
	for(i = 1; i < jobid; i++)
		if( i == max )
		{
			j++;
			max += max;
		}
			
	sprintf(pool_in,"%s/pool_in_%d",path,j);
	sprintf(pool_out,"%s/pool_out_%d",path,j);
				
	p_in = open(pool_in, O_WRONLY);
	p_out = open(pool_out, O_RDONLY | O_NONBLOCK);
	
	sprintf(buffer,"status %d",jobid);
	nwrite = write(p_in,buffer,strlen(buffer)+1);
	
	if( p_in == -1 || p_out == -1 || nwrite == -1 )
	{ 
		printf("Pool_%d finished.\n",j);
		if( errno == ENOENT || errno == EBADF ) // No such file then pool has finished and the job is also finished.
		{ 
			sprintf(buffer,"Jobid: %d Status: Finished",jobid);
			
			if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
			{ perror("Command status: Error writing to console"); }
			
			close(p_in);
			close(p_out);
			
			return 1;
		}
		perror("Command status");
		exit(1);
	}
		
	while(true)	
		if( (nread = read(p_out,buffer,sizeof(buffer))) > 0 )
		{
			printf("Received from pool: %s\n",buffer);
				
			if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
			{ perror("Command status: Error writing to console"); }
					
			break;
		}
				
	close(p_in);
	close(p_out);
	
	return 0;
}
