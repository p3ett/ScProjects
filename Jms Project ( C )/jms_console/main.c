#include "main.h"

int main(int argc, char* argv[])
{	
	int i = 1, count = 0, number = 0;
	int fd_in, fd_out, nwrite, nread;
	
	char* jms_in = "/home/p3ett/Documents/SySpro/project2/jms_coord/";
	char* jms_out = "/home/p3ett/Documents/SySpro/project2/jms_coord/";
	char* command;
	char buffer[100];
	
	char** str_check;
	
	FILE* fp;
	
	/* Check arguments and analyze */
    while( i < argc-1 )
    {
        if( strcmp(argv[i] , "-o") == 0 )
        {
            i++;
            count++;
            if( (fp = fopen(argv[i], "r")) == NULL ) 
            { 
            	perror("Error opening file");
		printf("Put commands manually:\n");
		fp = stdin;
            }
        }
        else if( strcmp(argv[i] , "-w") == 0 )
        {
   			i++;
      		count++;
      		jms_in = concat(jms_in,argv[i]);
        }
        else if( strcmp(argv[i] , "-r") == 0 )
        {
            i++;
            count++;
            jms_out = concat(jms_out,argv[i]);
        }
        i++;
   	}
    if( count != 3) { printf("Error: Arguments are missing!\n"); exit(8); }
	
	/* Open jms_in and jms_out	*/
	if( (fd_in = open(jms_in, O_WRONLY)) == -1 ) { perror("Open fifo failed!"); exit(1); }
	if( (fd_out = open(jms_out, O_RDONLY | O_NONBLOCK)) == -1 ) { perror("Open fifo failed!"); exit(2); }
	
	while(true)
	{	 	
		command = get_command(fp);
		str_check = analyze(command);
		
		if( feof(fp) == false )
		{
			if( strcmp(str_check[0],"submit") == 0 )
			{ 
				number++; 
					
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Message received: %s.\n",buffer);
						
						break;
					}
			}
			else if( strcmp(str_check[0],"status") == 0 )
			{
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Message received: %s.\n",buffer);
						
						break;
					}
			}
			else if( strcmp(str_check[0],"status-all") == 0 )
			{
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						if( strcmp(buffer,"end") == 0 ) { break; }
						
						printf("Message received: %s.\n",buffer);
					}
			}
			else if( strcmp(str_check[0],"show-active") == 0 )
			{
				int i = 1;
				
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				printf("Active Jobs:\n");
				while(true)
				{
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						if( strcmp(buffer,"end") == 0 ) { break; }
						printf("%d. %s\n",i,buffer);
						
						i++;
					}
				}
				
				if( i == 1 )
					printf("No active jobs.\n");
			}
			else if( strcmp(str_check[0],"show-finished") == 0 )
			{
				int i = 1;
				
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				printf("Finished Jobs:\n");
				while(true)
				{
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						if( strcmp(buffer,"end") == 0 ) { break; }
						printf("%d. %s\n",i,buffer);
						
						i++;
					}
				}
				
				if( i == 1 )
					printf("No finished jobs.\n");
			}
			else if( strcmp(str_check[0],"show-pools") == 0 )
			{
				int i = 1;
				
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				printf("Pool & NumOfJobs:\n");
			
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						if( strcmp(buffer,"end") == 0 ) { break; }
						printf("%d. %s\n",i,buffer);

						i++;
					}
				
			}
			else if( strcmp(str_check[0],"suspend") == 0 )
			{
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Message received: %s.\n",buffer);
						
						break;
					}
			}
			else if( strcmp(str_check[0],"resume") == 0 )
			{
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Message received: %s.\n",buffer);
						
						break;
					}
			}
			else if( strcmp(str_check[0],"shutdown") == 0 )
			{
				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
				printf("Message send: %s.\n",command);
				
				while(true)
					if( (nread = read(fd_out,buffer,sizeof(buffer))) > 0 )
					{
						printf("Message received: %s.\n",buffer);
						
						break;
					}
				free(command);
				free_table(str_check);
				break;
			}
			else if( strcmp(str_check[0],"exit") == 0 )
			{
				printf("console and coord are about to exit.\n");

				if( (nwrite = write(fd_in,command,strlen(command)+1)) == -1 ) { perror("Error writing to coord"); exit(3); }
                                printf("Message send: %s.\n",command);

				free(command);
				free(str_check);
				break;
			}
			else
			{
				printf("Wrong message!\n");
			}
					
			free(command);
			free_table(str_check);
		}
		else
		{
			fp = stdin;
			printf("Reach EOF.Put commands manually:\n");
		}
	}
	
	free(jms_in);
	free(jms_out);
	
	exit(0);
} 
