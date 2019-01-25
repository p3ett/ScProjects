#include "ContentServer.h"

void main(int argc, char** argv)
{
	int port, sock, newsock, end = false;
	int i = 0, count = 0;
	
	char* dirorfilename;
	
	signal(SIGCHLD, schild_handler);
	
	contentServer *cs;
	
	/*  Creating shared memory */
	int shm_id, shm1_id, shm2_id, sem_id, sem1_id, sem2_id, *sh, cs_id, *bytesTransferred, *filesTransferred;
	
	/* Create a new shared memory segment for sh  and attach it*/
	shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660);
	sh = (int*)shmat(shm_id, NULL, 0);
	printf("Creating shared memory for sh with ID: %d.\n", shm_id);
	
	/* Create a new shared memory segment for bytesTransferred and attach it */
	shm1_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660);
	bytesTransferred = (int*)shmat(shm1_id, NULL, 0);
	printf("Creating shared memory for bytesTransferred with ID: %d.\n", shm1_id);
	
	/* Create a new shared memory segment for filesTransferred and attach it */
	shm2_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0660);
	filesTransferred = (int*)shmat(shm2_id, NULL, 0);
	printf("Creating shared memory for filesTransferred with ID: %d.\n", shm2_id);
	
	/* Create a new shared memory segment for struct cs and attach it */
	cs_id = shmget(SHMKEY, sizeof(contentServer*), 0600 | IPC_CREAT);
	cs = (contentServer*) shmat(cs_id, NULL, 0);
	printf("Creating shared memory for struct cs with ID: %d.\n", cs_id);
	
	/* Create a new semaphore id for sh */
	sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0660);
	sem_Init(sem_id, 1);
	printf("Creating a semaphore with ID: %d.\n", sem_id);
	
	/* Create a new semaphore for bytesTransferred */
	sem1_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0660); 
	sem_Init(sem1_id, 1);
	printf("Creating a semaphore with ID: %d.\n", sem1_id);
	
	/* Create a new semaphore for filesTransferred, */
	sem2_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0660); 
	sem_Init(sem2_id, 1);
	printf("Creating a semaphore with ID: %d.\n", sem2_id);
	
	if( shm_id < 0 || shm1_id < 0 || shm2_id < 0 || cs_id < 0 || 
		sh == NULL || bytesTransferred == NULL || filesTransferred == NULL ||
		sem_id < 0 || sem1_id < 0 || sem2_id < 0 )
	{ 
		printf("Error on shared memory.\n");
		
		free_resources(shm_id, sem_id); free_resources(shm1_id, sem1_id); 
		free_resources(shm2_id, sem2_id); free_resources(cs_id, 0);
		
		exit(1);
	}
	
	/* Setting shared memory to 0 */
	*sh = 0;
	*bytesTransferred = 0;
	*filesTransferred = 0; 

	/* ------------------------------------------------------ */
	
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
        else if( strcmp(argv[i] , "-d") == 0 )
        {
			i++;
			count++;
			dirorfilename = argv[i];
        }
        
        i++;
   	}
   	if( count != 2) { printf("Error: Arguments are missing!\n"); exit(1); }	
   	
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
	
	int ch_num = 0; 
	pid_t *child_server;
	
	child_server = malloc(sizeof(pid_t)*ch_size);
	if( child_server == NULL ) { perror("malloc"); exit(1); }
	
	while( ch_num < ch_size )
	{
		printf(">> Listening for connections to port: %d.\n",port);

		/* Accept: Returns a new socket descriptor to communicate with client */
		if( (newsock = accept(sock,clientptr,&clientlen)) < 0 ) { perror("Accept"); exit(1); }

		/* Find clients address */
		if( (hp = gethostbyaddr(&client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL )
		{ herror("Gethostbyaddr"); exit(1); }
		
		if( read(newsock, &end, sizeof(end)) < 0 ) 
		{ perror("Read"); exit(1); }
		
		if( end == true ) { break; }
		
		if( (child_server[ch_num] = fork()) == -1 ) { perror("Fork"); break; }
		
		if( child_server[ch_num] == 0 ) 
		{ 		
			close(sock);
			
			printf("Child with: %d created.\n",getpid());
			
			server_assistant(newsock, dirorfilename, sem_id, sem1_id, sem2_id, sh, bytesTransferred, filesTransferred, cs);
			
			close(newsock);
			exit(0); 
		}
		
		ch_num++; // Get next child
	}
	
	if( ch_num == ch_size )
		printf("ContentServer cant create any more child_servers\n");
	
	printf("Waiting for childs to end.\n");
	
	int j;
	pid_t pid;
	
	while ( (pid = wait(NULL)) > 0 ) 
		for(j = 0; j <= ch_num; j++)
			if( pid == child_server[j] )
				printf(">> Child_server%d terminate successfully.\n",j);
				
	printf("Childs have ended. About to terminate ContentServer.\n");
	
	if( write(newsock, &(*bytesTransferred), sizeof(*bytesTransferred)) < 0 ) { perror("Write"); exit(1); }
	if( write(newsock, &(*filesTransferred), sizeof(*filesTransferred)) < 0 ) { perror("Write"); exit(1); }
	
	close(sock);
	close(newsock);
	
	free(child_server);
	
	/* Delete shared memory and semaphore */
	free_resources(shm_id, sem_id); free_resources(shm1_id, sem1_id); 
	free_resources(shm2_id, sem2_id); free_resources(cs_id, 0); 
	
	exit(0);
}
