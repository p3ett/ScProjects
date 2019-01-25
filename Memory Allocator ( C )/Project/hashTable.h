#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define SIZE1 8

typedef struct MemoryDataItem {
    uintptr_t page_address;
    unsigned char bit_vector[16];

    struct MemoryDataItem *next;
    struct MemoryDataItem *previous;
} MemoryDataItem;

int hash_code(size_t);
void hash_init(MemoryDataItem** );
MemoryDataItem * hash_insert_amount(MemoryDataItem **, size_t , uintptr_t );
MemoryDataItem * hash_search_amount(MemoryDataItem** , size_t );

void hash_display(MemoryDataItem** );



#endif