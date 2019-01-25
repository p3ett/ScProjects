#include "MirrorInitiator.h"

void main(int argc, char** argv)
{
	int MirrorServerPort, sock, argPos;
	int i = 0, count = 0;
	
	char* MirrorServerAddress;
	
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*) &server;
	
	struct hostent *hp;
	struct in_addr ip;
	
	/* Check arguments and analyze */
	while( i < argc-1 )
    {
    	if( strcmp(argv[i] , "-n") == 0 )
        {
			i++;
			count++;
			MirrorServerAddress = argv[i];
        }
        else if( strcmp(argv[i] , "-p") == 0 )
        {
            i++;
            count++;
            MirrorServerPort = atoi(argv[i]);
        }
        else if( strcmp(argv[i] , "-s") == 0 )
        {
            i++;
            count++;
            argPos = i; // Take the position of argv
        }
        
        i++;
   	}
   	if( count != 3) { printf("Error: Arguments are missing!\n"); exit(1); }	

   	/* Create socket */
	if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { perror("Socket creation"); exit(1); }
	
	/* if ip_check is true try with gethostbyaddr else with gethostbyname */
	if( ip_check(MirrorServerAddress) )
	{
		/* IPv4 dot - number into binary form ( network byte order ) */
		inet_aton(MirrorServerAddress,&ip);
		
		if( (hp = gethostbyaddr(&ip,sizeof(ip),AF_INET)) == NULL )
		{ herror("gethostbyaddr"); exit(1); }

	}else if( (hp = gethostbyname(MirrorServerAddress)) == NULL ) 
		  { herror("gethostbyname"); exit(1); }
   	
	/* Establish connection to server */
   	server.sin_family = AF_INET;
	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
	server.sin_port = htons(MirrorServerPort);
	
	if( connect(sock,serverptr,sizeof(server)) < 0 ) 
	{ 
		if( errno == ECONNREFUSED )
		{
			printf("No-one listening on the remote address.\n");
			exit(1);
		}
		else
		{
			perror("Connect"); 
			exit(1);
		} 
	}
	/* ----------------------------- */
	
	printf("Connect to server: %s from port: %d.\n",hp->h_name,MirrorServerPort);
	
	client_assistant(sock, argPos, argv);
 	
 	close(sock);
 	  	
	exit(0);
}
