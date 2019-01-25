#include "main.h"

void shutdown(pid_t pool[], char* path, int fd_out, int index, int max_jobs)
{
	int count = 0, ret, i;
	int nread, nwrite, p_out, p_in;
	
	char buffer[100];
	char pool_in[100];
	char pool_out[100];
	
	while( count < index )
	{	
		close(p_out);
		
		sprintf(pool_out,"%s/pool_out_%d",path,count);
		sprintf(pool_in,"%s/pool_in_%d",path,count);
		p_in = open(pool_in, O_WRONLY);
		p_out = open(pool_out, O_RDONLY | O_NONBLOCK);
		
		if( p_in == -1 || p_out == -1 || nwrite == -1 )
		{
			printf("Pool_%d finished.\n",i);
			if( errno == ENOENT || errno == EBADF )
			{ count++; continue; } // Go to next pool
			else
			{ perror("Command shutdown:"); exit(1); }
		}
		
		if( (ret = kill(pool[count],SIGTERM)) == -1 )
		{
			count++; // Pool doesnt exist
			continue; // Go to next
		}
		
		count++;

		sprintf(buffer,"shutdown");
		nwrite = write(p_in,buffer,strlen(buffer)+1);

		while(true)	
		if( (nread = read(p_out,buffer,sizeof(buffer))) > 0 )
		{	
			i += atoi(buffer);
			
			break;
		}
	}
	
	sprintf(buffer,"Served %d jobs, %d were still in progress",max_jobs*index,i);
	if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
	{ perror("Command status: Error writing to console"); }
	
	close(p_in);
	close(p_out);
}
