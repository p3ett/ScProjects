#include "main.h"

void exec_job(char** buffer, char* path, int number)
{	
	char absolute[100];
	int file1, file2;
	
	/* Making the directory */
	/* Get current time	*/
	time_t current_time;
	time(&current_time);
	struct tm * time_info = localtime(&current_time);
	char timeString[20];
	char otherString[20];
	
	/* Create the strings */
	strftime(timeString, sizeof(timeString), "%Y%m%d_%H%M%S", time_info);
	sprintf(otherString,"/jms_sdi1000218_%d_%d_",number,getpid());
	
	/* Stick the strings together */
	sprintf(absolute,"%s%s%s",path,otherString,timeString);
	
	if(mkdir(absolute,S_IRWXU | S_IRGRP | S_IROTH) == -1) { perror("mkdir"); }
	/* ---------------------------------------------------------------------- */
	
	/* Create the files	*/
	char sout[150];
	char serror[150];
	
	sprintf(sout,"%s/stdout_%d",absolute,number);
	sprintf(serror,"%s/stderr_%d",absolute,number);
	
	if( (file1 = open(sout, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ) { perror("Error opening file"); }
	if( (file2 = open(serror, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ) { perror("Error opening file"); }
	/*	----------------------------------------------------------------------------- */
	
	/* Redirecting input to files */
	dup2(file1,1);
	dup2(file2,2);
	/*	------------------------- */
	
	execvp(buffer[0],buffer);
	
	/* If this code occurs then exec failed */
	perror("Exec failed");
	exit(44);
}
