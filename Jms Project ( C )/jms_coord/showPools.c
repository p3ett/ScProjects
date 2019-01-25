#include "main.h"

void showPools(char* path, int fd_out, int index, pid_t pool[])
{
	int i = 0; 
	int p_in, p_out, nread, nwrite;
	
	char buffer[100];
	char pool_in[100];
	char pool_out[100];
	
	while( i < index )
	{
		close(p_in);
		close(p_out);
				
		/*	Change pool	*/
		sprintf(pool_in,"%s/pool_in_%d",path,i);
		sprintf(pool_out,"%s/pool_out_%d",path,i);
				
		p_in = open(pool_in, O_WRONLY);
		p_out = open(pool_out, O_RDONLY | O_NONBLOCK);
		
		sprintf(buffer,"show-pools");
		nwrite = write(p_in,buffer,strlen(buffer)+1);
		
		if( p_in == -1 || p_out == -1 || nwrite == -1 )
		{
			printf("Pool_%d Finished.\n",i);
			if( errno == ENOENT || errno == EBADF )
			{
				sprintf(buffer,"%d 0",pool[i]);
				if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
				{ perror("Command show-pools: Error writing to console"); }
							
				i++;
				continue;
			}
			else { perror("Command show-pools:"); exit(1); }
		}
				
		while(true)
			if( (nread = read(p_out,buffer,sizeof(buffer))) > 0 )
			{	
				printf("Received from pool: %s\n",buffer);
				
				if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
				{ perror("Command show-pools: Error writing to console"); }
							
				i++;
				break;
			}
	}
	sprintf(buffer,"end");
	if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
	{ perror("Command show-pools: Error writing to console"); }
				
	close(p_in);
	close(p_out);
}
