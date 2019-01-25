#include "main.h"

void showActive(char* path, int fd_out, int index)
{
	int i = 0;
	int p_in, p_out, nwrite, nread;
	
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
		
		sprintf(buffer,"show-active");
		nwrite = write(p_in,buffer,strlen(buffer)+1);
		
		if( p_in == -1 || p_out == -1 || nwrite == -1 )
		{
			printf("Pool_%d finished.\n",i);
			if( errno == ENOENT || errno == EBADF )
			{ i++; continue; } // Go to next pool
			else
			{ perror("Command show-active:"); exit(1); }
		}
				
		while(true)
		{
			if( (nread = read(p_out,buffer,sizeof(buffer))) > 0 )
			{
				printf("Received from pool: %s\n",buffer);
				
				if( strcmp(buffer,"end") == 0 ) { i++; break; }
				
				if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
				{ perror("Command show-active: Error writing to console"); }
			}
		}
	}
	sprintf(buffer,"end");
	if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
	{ perror("Command show-active: Error writing to console"); }
				
	close(p_in);
	close(p_out);
}
