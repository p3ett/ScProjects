#include "main.h"

char* get_command(FILE* fp)
{
    int i = 0;
    char ch_test;
    char buffer[256];

    ch_test = fgetc(fp);
    while( ch_test != EOF )
    {
        if( ch_test == '\n' )
            break;
        
        buffer[i] = ch_test;
        i++;
        ch_test = fgetc(fp);
    }
	buffer[i] = '\0';
	
	char* final = (char*)malloc(strlen(buffer)+1);
	if( final == NULL ) 
	{ 
		perror("Failed to malloc:"); 
		exit(10); 
	}

	strcpy(final,buffer);
	
    return final;
}

/* Split the string into tokens */
char** analyze(char* buffer)
{
	int i= 0;
	char hold[100];
	strcpy(hold,buffer);
	
	char* token;
	
	char** final = (char**)malloc(sizeof(char*)*10);
	if ( final == NULL ) { perror("Failed to malloc"); exit(10); }
	
	token = strtok(hold, " ");
	while( token != NULL )
	{
		final[i] = (char*)malloc(sizeof(char)*strlen(token)+1);
		if( final[i] == NULL ) { perror("Failed to malloc"); exit(11); }
		
		strcpy(final[i],token);
		
		i++;
		token = strtok(NULL, " ");
	}
	final[i] = NULL;

	return final;
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

char* concat(const char *s1, const char *s2)
{
    char* final = (char*)malloc(strlen(s1)+strlen(s2)+1);
    if( final == NULL )
    {
    	perror("Failed to malloc:");
    	exit(11);
    }
    
    strcpy(final, s1);
    strcat(final, s2);
    return final;
}


