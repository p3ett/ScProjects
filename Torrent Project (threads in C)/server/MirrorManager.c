#include "MirrorServer.h"

void *thread_start(void* arg)
{	
	char buffer[256], hold[256], ch[1];
	
	char* str = (char*) arg;

	/* Analyze string and create the right variables for its tokens */
	char** strCheck = analyze(str,":",strlen(str)+1);
	char** strTable;
	
	printf("MirrorManager: %ld Received string: %s\n",pthread_self(),str);

	free(str); // We are done with th_str

	/* Save the contexts from analyze */
	int i = 0, sock, size, nread, received, id, dir_or_not, count1 = 0, count2 = 0, end2 = false;
	int ContentServerPort = atoi(strCheck[1]), delay = atoi(strCheck[3]);
	
	char* ContentServerAddress = (char*)malloc(sizeof(char)*strlen(strCheck[0])+1);
	if ( ContentServerAddress == NULL ) { perror("Failed to malloc"); pthread_mutex_unlock(&lock); pthread_exit(NULL); }
	
	strncpy(ContentServerAddress, strCheck[0], strlen(strCheck[0])+1);

	char* dirorfile = (char*)malloc(sizeof(char)*strlen(strCheck[2])+1);
	if ( dirorfile == NULL ) { perror("Failed to malloc"); pthread_mutex_unlock(&lock); pthread_exit(NULL); }
	
	strncpy(dirorfile, strCheck[2], strlen(strCheck[2])+1);
	/* --------------------------------------------------------------- */
	
	/* Save address and port for future connections */
	fut_con[numDevices].ContentPort = ContentServerPort;
	
	fut_con[numDevices].ContentAdrress = (char*) malloc(sizeof(char)*strlen(ContentServerAddress)+1);
	strcpy(fut_con[numDevices].ContentAdrress, ContentServerAddress);
	
	numDevices++;
	/* ------------------------------------------- */
	
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*) &server;
	
	struct hostent *hp;
	struct in_addr ip;

	/* Create socket */
	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
	{ perror("Socket creation"); pthread_mutex_unlock(&lock); pthread_exit(NULL); }
	
	/* if ip_check is true try with gethostbyaddr else with gethostbyname */
	if( ip_check(ContentServerAddress) )
	{
		/* IPv4 dot - number into binary form ( network byte order ) */
		inet_aton(ContentServerAddress,&ip);
		
		if( (hp = gethostbyaddr(&ip,sizeof(ip),AF_INET)) == NULL )
		{ herror("gethostbyaddr"); pthread_mutex_unlock(&lock); pthread_exit(NULL); }

	}else if( (hp = gethostbyname(ContentServerAddress)) == NULL ) 
		  { herror("gethostbyname"); pthread_mutex_unlock(&lock); pthread_exit(NULL); }
   	
	/* Establish connection to server */
   	server.sin_family = AF_INET;
	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
	server.sin_port = htons(ContentServerPort);

	if( connect(sock,serverptr,sizeof(server)) < 0 ) 
	{ 
		if( errno == ECONNREFUSED )
		{
			printf(">> MirrorManager: %ld: No-one listening on the remote address.\n",pthread_self());
			pthread_mutex_unlock(&lock); 
			pthread_exit(NULL);
		}
		else
		{
			perror("Connect"); 
			pthread_mutex_unlock(&lock); 
			pthread_exit(NULL);
		} 
	}
	/* ----------------------------- */ 
	
	/* end = false so proceed */
	if( write(sock, &end2, sizeof(end2)) < 0 ) { perror("Write"); exit(1); }
	
	/* Send LIST to content server */
	id = pthread_self()%10;

	sprintf(buffer, "LIST,%d,%d", id, delay);
	
	size = strlen(buffer)+1;
	if( write(sock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }
		
	write_all(sock, buffer, strlen(buffer)+1);
	/* ---------------------------------------------- */

	while( true )
	{
		received = 0;
		
		/* Check if what you read is a directory */
		if( read(sock, &dir_or_not, sizeof(dir_or_not)) < 0 ) 
		{ perror("Read"); pthread_exit(NULL); }
		
		/* Read the desired readable size */
		if( read(sock, &size, sizeof(size)) < 0 ) 
		{ perror("Read"); pthread_exit(NULL); }
	
		/* Read all data */
		while( received < size )
		{
			if( (nread = read(sock, buffer + received, size - received)) < 0 )
			{ perror("Read"); pthread_exit(NULL); }
		
			received += nread;
		}
		
		if( strncmp(buffer, "end", 4) == 0 ) { break; }
		
		count2++;
		
		strTable = analyze(buffer, "/", strlen(buffer)+1);
		
		if( dir_exists(strTable, dirorfile) == true )
		{
			/* Place when not full and wake up worker if you place something */
			pthread_mutex_lock(&cond_lock);
		
			while( queue_size >= QUEUE_SIZE )
			{
				printf(">> MirrorManager %ld: Queue is full. I will wait.\n", pthread_self());
				pthread_cond_wait(&cond_full, &cond_lock);
			}
		
			sprintf(hold, "%s,%s,%d,%d,%d", buffer, ContentServerAddress, ContentServerPort, id, dir_or_not);
		
			printf(">> MirrorManager: %ld: Add string: %s\n",pthread_self(),hold);
		
			Enqueue(Q, hold);
		
			pthread_cond_signal(&cond_empty);
		
			pthread_mutex_unlock(&cond_lock);
			/* --------------------------------------------------------- */
		}
		else
			count1++;
		
		free_table(strTable);
	}
	
	if( count1 == count2 )
		printf(">> MirrorManager: %ld: ContentServer does not support the folder: %s.\n",pthread_self(),dirorfile);
	
	close(sock);	
	
	free(dirorfile);
	free(ContentServerAddress);

	free_table(strCheck);

	pthread_exit(NULL);
}
