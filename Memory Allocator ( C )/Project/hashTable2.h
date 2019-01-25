#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "hashTable.h"

#define SIZE2 10000

typedef struct MemoryDataItem2 {
    uintptr_t page_address;
    size_t size;
    struct MemoryDataItem *pointerToMDR;

    struct MemoryDataItem2 *next;
    struct MemoryDataItem2 *previous;
} MemoryDataItem2;

int hash2_code(uintptr_t );
void hash2_init(MemoryDataItem2** );
MemoryDataItem2 * hash2_insert_address(MemoryDataItem2 **, MemoryDataItem *, size_t);
MemoryDataItem2 * hash2_search_address(MemoryDataItem2 **, uintptr_t page_address);

void hash2_display(MemoryDataItem2* );

