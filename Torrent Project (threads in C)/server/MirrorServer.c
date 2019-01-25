#include "MirrorServer.h"

void main(int argc, char** argv)
{
	int port, threadnum, sock, newsock, err, size = 0, finish;
	int i = 0, count = 0;
	
	numDevices = 0; // Global
	
	pthread_cond_init(&cond_full, NULL);
	pthread_cond_init(&cond_empty, NULL);
	pthread_mutex_init(&cond_lock, NULL);
	
	/* sin_family,sin_port,sin_addr */
	struct sockaddr_in server, client;

	/* sa_family, sa_data (address family,protocol) */
	struct sockaddr *serverptr = (struct sockaddr *) &server;  
	struct sockaddr *clientptr = (struct sockaddr *) &client; 
	
	struct hostent *hp;
	
	socklen_t clientlen = sizeof(client);
	
	/* Check arguments and analyze */
	while( i < argc-1 )
    {
    	if( strcmp(argv[i] , "-p") == 0 )
        {
			i++;
			count++;
			port = atoi(argv[i]);
        }
        else if( strcmp(argv[i] , "-m") == 0 )
        {
			i++;
			count++;
			path = argv[i];
        }
        else if( strcmp(argv[i] , "-w") == 0 )
        {
			i++;
			count++;
			threadnum = atoi(argv[i]);
        }
        
        i++;
   	}
   	if( count != 3) { printf("Error: Arguments are missing!\n"); exit(1); }	
   	
   	/* Create directory */
   	if(mkdir(path,S_IRWXU | S_IRGRP | S_IROTH) == -1) { }
   	
   	/* Create socket */
	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{ perror("Socket creation"); }
	
	/* Create bind socket to address*/
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port); 
	
	if( bind(sock, serverptr, sizeof(server)) < 0 ) { perror("Bind creation"); exit(1); }
	/* ------------------------------ */
	
	/* Listen for connections */
	if( listen(sock,5) < 0 ) { perror("listen"); exit(1); }
	
	end == false;
		
	/* Create as much Workers as threadnum */
	pthread_t *Worker;
	
	Worker = malloc(sizeof(pthread_t)*threadnum);
	if( Worker == NULL ) { perror("malloc"); exit(1); }
	
	Q = queue_init();
	
	for(i = 0; i < threadnum; i++)
		if( (err = pthread_create(&Worker[i], NULL, thread_worker, (void*) NULL)) == true )
		{ show_error("pthread_create", err); }
	/* -------------------------------------- */

	while(true)
	{
		printf("Listening for connections to port: %d.\n",port);
		
		/* Accept: Returns a new socket descriptor to communicate with client */
		if( (newsock = accept(sock,clientptr,&clientlen)) < 0 ) { perror("Accept"); exit(1); }

		/* Find clients address */
		if( (hp = gethostbyaddr(&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL )
		{ herror("Gethostbyaddr"); exit(1); }

		printf("MirrorServer: Accepted connection from: %s.\n",hp->h_name);
		
		server_assistant(newsock);

		if( read(newsock, &finish, sizeof(finish)) < 0 ) 
		{ perror("Read"); exit(1); }
		
		printf("MirrorServer: Closing connection with: %s.\n",hp->h_name);
		printf("------------------------end connection---------------------\n");
		
		if( finish == true )
			break;
		else
			close(newsock);
	}
	
	/* wake up all workers and tell them to finish */
	while( true )
		if( queue_size <= 0 )
		{
			end = true;
			pthread_cond_broadcast(&cond_empty);
			break;
		}

	/* Wait for all Workers to finish */
	for(i = 0; i < threadnum; i++)
	{
		if( (err = pthread_join(Worker[i], NULL)) == true )
		{ show_error("pthread_join", err); }
	}
	
	terminator(newsock);
	
	close(newsock);
	
	free(Q->elements);
	free(Q);
	
	free(Worker);
	
	pthread_cond_destroy(&cond_full);
	pthread_cond_destroy(&cond_empty);
	pthread_mutex_destroy(&cond_lock);
	
	close(sock);
	
	exit(0);
}
