#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>

#define true 1
#define false 0

/* Decleration of functions*/

/* functions.c */
char* get_command(FILE* );
char* concat(const char* , const char* );

char** analyze(char* );
void free_table(char** );
