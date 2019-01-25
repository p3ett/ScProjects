#include "ContentServer.h"

void server_assistant(int newsock, char* dirorfilename, int sem_id, int sem1_id, int sem2_id, int *sh, int* bytesTransferred, int* filesTransferred, contentServer *cs)
{
	int size, i, received = 0, nread, id_check, file;

	char ch;
	char buffer[256];
	
	/* Read the desired readable size */
	if( read(newsock, &size, sizeof(size)) < 0 ) 
	{ perror("Read"); exit(1); }
	
	/* Read all data */
	while( received < size )
	{
		if( (nread = read(newsock, buffer + received, size - received)) < 0 )
		{ perror("Read"); exit(1); }
		
		received += nread;
	}
	
	char** strTable = analyze(buffer, ",", strlen(buffer)+1);
	
	if( strncmp(strTable[0], "LIST", 4) == 0 )
	{	
		printf(">> Accepted connection from MirrorManager.\n");
		
		/* Change values of shared memory */
		
		sem_P(sem_id);
		
		cs[*sh].id = atoi(strTable[1]);
		cs[*sh].delay = atoi(strTable[2]);
		
		(*sh)++;
		
		sem_V(sem_id);
		
		/* ---------------------------- */
		
		//sanitize(dirorfilename);
		snprintf(buffer, sizeof(buffer), "ls %s", dirorfilename);
		
		/* Write the list of all folders and files that ContentServer offers to copy */
		make_list(newsock, buffer, dirorfilename);

		/* Send end to MirrorManager because client is about to finish */
		size = 4;
		
		if( write(newsock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }
		if( write(newsock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }
		write_all(newsock, "end", 4);
		/* ---------------------------------------------------------- */
		
		printf("------------end connection with MirrorManager---------------\n");
	}
	else if( strcmp(strTable[0], "FETCH") == 0 )
	{
		printf(">> Accepted connection from Worker.\n");

		/* ContentServer sleeps delay seconds */
		id_check = atoi(strTable[2]);

		for(i = 0; i < *sh; i++)
			if( id_check == cs[i].id )
			{
				printf("I am child: %d and i will sleep: %d. secs\n",getpid(),cs[i].delay);
				sleep(cs[i].delay);
				break;
			}

		if( (file = open(strTable[1], O_RDONLY)) == -1 ) { perror("Open"); }

		/* Raise bytesTransferred */
		sem_P(sem1_id);

		struct stat st;
		stat(strTable[1], &st);
		size = st.st_size;

		(*bytesTransferred) += size;

		sem_V(sem1_id);
		/*  ------------------- */
		
		while( (nread = read(file, buffer, sizeof(buffer))) > 0 )
		{
			if( write(newsock, &nread, sizeof(nread)) < 0 ) { perror("Write"); exit(1); }
			
			write_all(newsock, buffer, nread);
		}
		
		size = 4;
		if( write(newsock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }
		write_all(newsock, "end", 4);
		
		/* Raise filesTransferred */
		sem_P(sem2_id);
		
		(*filesTransferred)++;

		sem_V(sem2_id);
		/* --------------------- */
		
		close(file);	
		printf("----------------end connection with Worker-----------------\n");
	}
	
	free_table(strTable);
}
