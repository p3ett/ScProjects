#include "MirrorServer.h"

/* Check if MirrorServerAddress is ip or host name */
int ip_check(char* buffer)
{
	int i = 0;
	char hold[100];
	
	strcpy(hold,buffer);
	
	char* token;
	
	token = strtok(hold, ".");
	while( token != NULL )
	{
		while( token[i] != '\0')
			if( isdigit(token[i]) )
				i++;
			else
				return false;
		
		i++;		
		token = strtok(NULL, " ");
	}
	
	return true;
}

/* Split the string into tokens */
char** analyze(char* buffer, char* delimiter, int size)
{
	int i= 0;
	char hold[size];

	strcpy(hold,buffer);
	
	char* token;
	
	char** final = (char**)malloc(sizeof(char*)*10);
	if ( final == NULL ) { perror("Failed to malloc"); exit(1); }
	
	token = strtok(hold, delimiter);
	while( token != NULL )
	{
		final[i] = (char*)malloc(sizeof(char)*strlen(token)+1);
		if( final[i] == NULL ) { perror("Failed to malloc"); exit(1); }
		
		strcpy(final[i],token);
		
		i++;
		token = strtok(NULL, delimiter);
	}
	final[i] = NULL;

	return final;
}

int dir_exists(char** strTable, char* dirorfile)
{
	int i = 0;
	char hold[256];
	char hold2[256];
	
	sprintf(hold, "/%s",strTable[i]);
		
	while( strTable[i] != NULL )
	{
		if( strcmp(hold, dirorfile) == 0 )
			return true;
	
		i++;
		
		sprintf(hold, "%s/%s",hold,strTable[i]);
	}
	
	return false;
}

int isDirectory(const char *path) 
{
   struct stat statbuf;

   if( stat(path, &statbuf) != 0 )
   { perror("Stat"); return -1; }

   return S_ISDIR(statbuf.st_mode);
}

void free_table(char** table)
{
	int i = 0;

	while( table[i] != NULL )
	{
		free(table[i]);
    	i++;
  	}

	free(table);
}

void show_error(char* str, int err)
{
	fprintf(stderr, "%s: %s\n",str, strerror(err));
	exit(1);
}

int write_all(int sock, char* buffer, int size)
{
	int nwrite, sent = 0;
	
	while( sent < size )
	{
		if( (nwrite = write(sock, buffer + sent, size - sent)) < 0 ) 
		{ perror("Write"); exit(1); }
		
		sent += nwrite;
	}
	
	return sent;
}
