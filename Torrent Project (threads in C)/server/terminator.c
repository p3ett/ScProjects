#include "MirrorServer.h"

void terminator(int newsock2)
{
	int i = 0, sock, size, filesTransferred, bytesTransferred;
	
	while( i < numDevices )
	{
		struct sockaddr_in server;
		struct sockaddr *serverptr = (struct sockaddr*) &server;
	
		struct hostent *hp;
		struct in_addr ip;

		/* Create socket */
		if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
		{ perror("Socket creation"); exit(1); }
	
		/* if ip_check is true try with gethostbyaddr else with gethostbyname */
		if( ip_check(fut_con[i].ContentAdrress) )
		{
			/* IPv4 dot - number into binary form ( network byte order ) */
			inet_aton(fut_con[i].ContentAdrress,&ip);
		
			if( (hp = gethostbyaddr(&ip,sizeof(ip),AF_INET)) == NULL )
			{ herror("gethostbyaddr"); exit(1); }

		}else if( (hp = gethostbyname(fut_con[i].ContentAdrress)) == NULL ) 
		 	 { herror("gethostbyname"); exit(1); }
   	
		/* Establish connection to server */
   		server.sin_family = AF_INET;
		memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
		server.sin_port = htons(fut_con[i].ContentPort);

		if( connect(sock,serverptr,sizeof(server)) < 0 ) 
		{ perror("Connect"); exit(1); }
		
		/* Terminate ContentServer */
		if( write(sock, &end, sizeof(end)) < 0 ) { perror("Write"); exit(1); }
		
		printf("Sent terminate signal to host: %s\n",fut_con[i].ContentAdrress);
		
		if( read(sock, &bytesTransferred, sizeof(bytesTransferred)) < 0 ) { perror("Read"); exit(1); }
		if( read(sock, &filesTransferred, sizeof(filesTransferred)) < 0 ) { perror("Read"); exit(1); }
		
		if( write(newsock2, &bytesTransferred, sizeof(bytesTransferred)) < 0 ) { perror("Write"); exit(1); }
		if( write(newsock2, &filesTransferred, sizeof(filesTransferred)) < 0 ) { perror("Write"); exit(1); }
		
		free(fut_con[i].ContentAdrress);
		
		i++;
	}
}
