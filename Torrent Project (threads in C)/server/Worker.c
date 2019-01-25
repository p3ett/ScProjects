#include "MirrorServer.h"

void *thread_worker(void* arg)
{	
	int size, sock, ContentServerPort, id, dir_or_not, file, received, nread, end2 = false;
	
	char buffer[512];
	char absolute[256];
	char absolute2[256];
	
	char* dirorfilename;
	char* ContentServerAddress;
	
	char** strTable;
	char** strCheck;
	
	/* Structs for connection */
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*) &server;
	
	struct hostent *hp;
	struct in_addr ip;
	
	while( end == false )
	{
		/* Remove from queue and wake up MirrorManager if he sleeps */
		pthread_mutex_lock(&cond_lock);
		
		while( queue_size <= 0 && end == false )
		{
			printf(">> Worker %ld: Queue is empty. I will wait.\n",pthread_self());
			pthread_cond_wait(&cond_empty, &cond_lock);
		}

		if( end == true && queue_size <= 0 ) 
		{
			pthread_mutex_unlock(&cond_lock);
			break; 
		}
		else
		{
			strTable = analyze(front(Q), ",", strlen(front(Q))+1);
		
			printf(">> Worker %ld: Remove string %s\n",pthread_self(),front(Q));
			Dequeue(Q);

			pthread_cond_signal(&cond_full);
	
			pthread_mutex_unlock(&cond_lock);
			/* ------------------------------------------------------- */
		
			/* Save the contexts from analyze */
			dirorfilename = (char*) malloc(sizeof(char)*strlen(strTable[0])+1);
			strncpy(dirorfilename, strTable[0], strlen(strTable[0])+1);
		
			ContentServerAddress = (char*) malloc(sizeof(char)*strlen(strTable[1])+1);
			strncpy(ContentServerAddress, strTable[1], strlen(strTable[1])+1);
		
			ContentServerPort = atoi(strTable[2]);
		
			id = atoi(strTable[3]);
		
			dir_or_not = atoi(strTable[4]);
			/* ---------------------------------------------- */
		
			/* Make directory dirname/content_name */
			sprintf(absolute, "%s/%s,%d",path,ContentServerAddress,ContentServerPort);
			if(mkdir(absolute,S_IRWXU | S_IRGRP | S_IROTH) == -1) { }

			/* Keep only the name of dirorfilename not the whole path */
			strCheck = analyze(dirorfilename, "/", strlen(dirorfilename)+1);
			
			int i = 0;
			while( strCheck[i] != NULL )
				i++;
			/* ------------------------------------------------- */
	
			if( dir_or_not == 1 ) // We have a directory
			{	
				sprintf(absolute2, "%s/%s",absolute,strCheck[i-1]);
	
				if(mkdir(absolute2,S_IRWXU | S_IRGRP | S_IROTH) == -1) { }
			}
			else // We have a file
			{
				/* Create socket */
				if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
				{ perror("Socket creation"); pthread_exit(NULL); }
	
				/* if ip_check is true try with gethostbyaddr else with gethostbyname */
				if( ip_check(ContentServerAddress) )
				{
					/* IPv4 dot - number into binary form ( network byte order ) */
					inet_aton(ContentServerAddress,&ip);
		
					if( (hp = gethostbyaddr(&ip,sizeof(ip),AF_INET)) == NULL )
					{ herror("gethostbyaddr"); pthread_exit(NULL); }

				}else if( (hp = gethostbyname(ContentServerAddress)) == NULL ) 
					  { herror("gethostbyname"); pthread_exit(NULL); }
   	
				/* Establish connection to server */
	   			server.sin_family = AF_INET;
				memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
				server.sin_port = htons(ContentServerPort);

				if( connect(sock,serverptr,sizeof(server)) < 0 ) 
				{ perror("Connect"); pthread_exit(NULL); }
				/* ----------------------------- */ 

				/* end = false so proceed */
				if( write(sock, &end2, sizeof(end2)) < 0 ) { perror("Write"); exit(1); }

				/* Send FETCH to content server */
				sprintf(buffer, "FETCH,%s,%d",dirorfilename,id);
		
				size = strlen(buffer)+1;
				if( write(sock, &size, sizeof(size)) < 0 ) { perror("Write"); exit(1); }
		
				write_all(sock, buffer, strlen(buffer)+1);
				/* --------------------------------- */
			
				sprintf(absolute, "%s/%s/%s",absolute,strCheck[i-2],strCheck[i-1]);
			
				/* Try to open the file but if errno=2 then the file its not inside a folder */
				if( (file = open(absolute, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 )
				{  
					if( errno == 2 )
					{
						sprintf(absolute, "%s/%s,%d/%s",path, ContentServerAddress, ContentServerPort, strCheck[i-1]);
					
						if( (file = open(absolute, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 )
						{ perror("Open"); }
					}
				else
					perror("Open second");
				}

				while( true )
				{
					received = 0;
				
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
				
					write_all(file, buffer, received);
				}	
			
			close(sock);
			close(file);
			}
		
		free(dirorfilename);
		free(ContentServerAddress);
		
		free_table(strTable);
		free_table(strCheck);
		}
	}
	
	printf("Worker is about to terminate.\n");
	pthread_exit(NULL);
}
