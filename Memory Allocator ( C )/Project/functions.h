#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

void * mymalloc(size_t memory_size);

void myfree(void * memory_address);

int myPow(int x,int n);

size_t myround(size_t memory_amount);
