#include "main.h"


int main(int argc, char* argv[])
{	
	int i = 1, count = 0, index = 0, number = 1;
	int max_jobs, nread, nwrite;
	int fd_in, fd_out, fpool_in, fpool_out;
	
	char* path;
	char* jms_in;
	char* jms_out;
	
	char** str_check;
	
	char buffer[100];
	char pool_in[100];
	char pool_out[100];
	
	pid_t pool[size];
	
	/* Check arguments and analyze */
	while( i < argc-1 )
    {
    	if( strcmp(argv[i] , "-l") == 0 )
        {
			i++;
			count++;
			path = argv[i];
        }
        else if( strcmp(argv[i] , "-n") == 0 )
        {
            i++;
            count++;
            max_jobs = atoi(argv[i]);
        }
        else if( strcmp(argv[i] , "-w") == 0 )
        {
   			i++;
      		count++;
      		jms_out = argv[i];	
        }
        else if( strcmp(argv[i] , "-r") == 0 )
        {
            i++;
            count++;
            jms_in = argv[i];
        }
        i++;
   	}
   	if( count != 4) { printf("Error: Arguments are missing!\n"); exit(8); }
   	
	/* Create directory */
   	if(mkdir(path,S_IRWXU | S_IRGRP | S_IROTH) == -1) { perror("mkdir"); }
   	
	/* Disconnect any fifo that exists */
   	disconnect(jms_in,jms_out);

	/* Create and Open jms_in and jms_out */
	if( mkfifo(jms_in, 0666) == -1 ) { perror("Make fifo for jms_in failed!"); exit(1); }
	if( mkfifo(jms_out, 0666) == -1 ) { perror("Make fifo for jms_out failed!"); exit(2); }
	if( (fd_in = open(jms_in, O_RDONLY | O_NONBLOCK)) == -1 ) { perror("Open fifo for jms_in failed!"); exit(3); }
	if( (fd_out = open(jms_out, O_WRONLY)) == -1 ) { perror("Open fifo for jms_out failed!"); exit(4); }
	/* ****************************** */ 
	
	printf("Create jms_coord with pid: %d\n",getpid());

	while(true)
	{	
		/*	Read from jms_console */
		if( (nread = read(fd_in,buffer,sizeof(buffer))) > 0 )
		{
			printf("Coord: message received: %s.\n",buffer);
			
			str_check = analyze(buffer);
			
			if( strcmp(str_check[0],"submit") == 0 ) 
			{ 
				if( count == max_jobs || index == 0 )
				{	
					pool[index] = fork();
				
					if( pool[index] == 0 ) { pool_process(path,max_jobs,index,number); }
	
					sleep(1);
				
					close(fpool_in);
					close(fpool_out);
				
			   		/* Stick the path behind pool_in and pool_out */
					sprintf(pool_in,"%s/pool_in_%d",path,index);
					sprintf(pool_out,"%s/pool_out_%d",path,index);
				
					/* Open pool_in and pool_out to connect with pool */
					if( (fpool_in = open(pool_in, O_WRONLY)) == -1 ) 
					{ perror("Command submit: Open fifo for pool_in failed!"); exit(15); }
					
					if( (fpool_out = open(pool_out, O_RDONLY | O_NONBLOCK)) == -1 ) 
					{ perror("Command submit: Open fifo for pool_out failed!"); exit(16); }
				
					index++;
					count = 0;
				}
			
				count++; 
				number++; 
				
				if( (nwrite = write(fpool_in,buffer,strlen(buffer)+1)) == -1 ) 
				{ perror("Command submit: Error writing to pool"); }
			
				while(true)
					if( (nread = read(fpool_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Received from pool: %s\n",buffer);
						
						if( (nwrite = write(fd_out,buffer,strlen(buffer)+1)) == -1 ) 
						{ perror("Command submit: Error writing to console"); }
					
						break;
					}
			}
			else if( strcmp(str_check[0],"status") == 0 )
			{
				int jobid = atoi(str_check[1]);
				
				status(path,fd_out,jobid,max_jobs); 
			}
			else if( strcmp(str_check[0],"status-all") == 0 )
			{
				status_all(path,fd_out,index,max_jobs);
			}
			else if( strcmp(str_check[0],"show-active") == 0 )
			{
				showActive(path,fd_out,index);
			}
			else if( strcmp(str_check[0],"show-finished") == 0 )
			{
				showFinished(path,fd_out,index,max_jobs);
			}
			else if( strcmp(str_check[0],"show-pools") == 0 )
			{
				showPools(path,fd_out,index,pool);
			}
			else if( strcmp(str_check[0],"suspend") == 0 )
			{
				int jobid = atoi(str_check[1]);
				
				suspend(path,fd_out,jobid,max_jobs); 
			}
			else if( strcmp(str_check[0],"resume") == 0 )
			{
				int jobid = atoi(str_check[1]);
				
				resume(path,fd_out,jobid,max_jobs); 
			}
			else if( strcmp(str_check[0],"shutdown") == 0 )
			{
				shutdown(pool,path,fd_out,index,max_jobs);
				free(str_check);
				break;
			}
			else if( strcmp(str_check[0],"exit") == 0 )
			{
				printf("Coord about to exit.\n");
				free(str_check);
                                break;
			}
			
			free(str_check);
		}
	}
	
	int j;
	pid_t pid;
	
	while ( (pid = wait(NULL)) > 0 ) 
		for(j = 0; j <= index; j++)
			if( pid == pool[j] )
				printf("Coord: pool_%d terminate successfully.\n",j);
			
	
	printf("jms_coord exiting.\n");
	
	/* If coord terminate unlick it's fifo */
	disconnect(jms_in,jms_out);
	
	exit(0);
}
