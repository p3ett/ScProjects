#include "MirrorInitiator.h"

void client_assistant(int sock, int argPos, char** argv)
{
	int i = 0, size, end = true, filesTransferred, bytesTransferred; 
	
	char buffer[256];
	
	char** StrTable;
	
	content_num = 0; // Global variable
		
	StrTable = analyze(argv[argPos], ",", strlen(argv[argPos]));
	
	/* Send how much contentServers exists */
	if( write(sock, &content_num, sizeof(content_num)) < 0 ) { perror("Write"); exit(1); }
	
	/* Send data to server */
	while( StrTable[i] != NULL )
	{
		/* Sent the size of data that server has to read */	
		size = strlen(StrTable[i])+1;
		if( write(sock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }

		/* write all the data to MirrorManager */
		write_all(sock, StrTable[i], strlen(StrTable[i])+1);

		i++; // Take next string
	}
	/* -------------------- */
	
	free_table(StrTable);
	
	while( strcmp(buffer, "yes") != 0 && strcmp(buffer, "no") != 0)
	{
		printf("Do you want to shut down MirrorServer? [yes/no]\n");
		scanf("%s",buffer);
	
		if( strcmp(buffer, "yes") == 0 )
		{
			if( write(sock, &end, sizeof(end)) < 0 ) { perror("Write"); exit(1); }
			
			for(i = 0; i < content_num; i++)
			{
				if( read(sock, &bytesTransferred, sizeof(bytesTransferred)) < 0 ) { perror("Read"); exit(1); }
				if( read(sock, &filesTransferred, sizeof(filesTransferred)) < 0 ) { perror("Read"); exit(1); }
			
				printf("Total files that transferred are: %d.\n",filesTransferred);
				printf("Total bytes that transferred are: %d.\n",bytesTransferred);
			}
		}
	}
}
