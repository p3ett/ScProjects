#include "main.h"

void status_all(char* path, int fd_out, int index, int max_jobs)
{
	int i = max_jobs;
	int count = 0; 
	int p_in, p_out, nwrite, nread;
	
	char buffer[100];
	char pool_in[100];
	char pool_out[100];
				
	while( count < index )
	{
		if( i == max_jobs )
		{
			i = 0;
						
			close(p_in);
			close(p_out);
						
			/*	Change pool	*/
			sprintf(pool_in,"%s/pool_in_%d",path,count);
			sprintf(pool_out,"%s/pool_out_%d",path,count);
			
			p_in = open(pool_in, O_WRONLY);
			p_out = open(pool_out, O_RDONLY | O_NONBLOCK);
			
			sprintf(buffer,"status-all");
			nwrite = write(p_in,buffer,strlen(buffer)+1);
			
			if( p_in == -1 || p_out == -1 || nwrite == -1 )
			{
				printf("Pool_%d Finished.\n",count);
				if( errno == ENOENT || errno == EBADF )
				{
					int jobs, id;
					id = max_jobs*count;
								
					for(jobs = 1; jobs <= max_jobs; jobs++)
					{
						sprintf(buffer,"Jobid: %d Status: Finished",jobs+id);
						
						if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
						{ perror("Command status-all: Error writing to console"); }
					}
							
					count++;
					i = max_jobs;
					continue;
				}
				else { perror("Command status-all:"); exit(1); }
			}
		}
		
		while(true)
		if( (nread = read(p_out,buffer,sizeof(buffer))) > 0 )
		{
			i++;
			printf("Received from pool: %s\n",buffer);
			
			if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
			{ perror("Command status-all: Error writing to console"); }
			
			if( i == max_jobs ) { count++; break; }
		}
	}
	
	sprintf(buffer,"end");
	if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
	{ perror("Command status-all: Error writing to console"); }
	
	close(p_in);
	close(p_out);
}
