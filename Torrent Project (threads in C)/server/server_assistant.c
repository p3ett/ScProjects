#include "MirrorServer.h"

void server_assistant(int newsock)
{		
	int th_size, err, size, received, nread, i;

	char* dirname;
	char* th_str;

	char buffer[256];
	
	/* As much MirrorManager as contentServers */
	if( read(newsock, &th_size, sizeof(th_size)) < 0 )
	{ perror("Read"); exit(1); }

	pthread_t *MirrorManager;
	
	MirrorManager = malloc(sizeof(pthread_t)*th_size);
	if( MirrorManager == NULL ) { perror("malloc"); exit(1); }
	/* -------------------------------------- */
	
	int th_num = 0;	
	
	while( th_num < th_size )
	{	
		received = 0;

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

		/* Create MirrorManagers */
		th_str = (char*) malloc(sizeof(char)*strlen(buffer)+1);
		strncpy(th_str, buffer, strlen(buffer)+1);

		if( (err = pthread_create(&MirrorManager[th_num], NULL, thread_start, (void*) th_str)) == true )
		{ show_error("pthread_create", err); }
		/* -------------------------------------------------- */
		
		/* Take next MirrorManager */
		th_num++;
	}

	/* Wait for all MirrorManagers to finish */
	for(th_num = 0; th_num < th_size; th_num++)
	{
		if( (err = pthread_join(MirrorManager[th_num], NULL)) == true )
		{ show_error("pthread_join", err); }
	}
	
	free(MirrorManager);
}
