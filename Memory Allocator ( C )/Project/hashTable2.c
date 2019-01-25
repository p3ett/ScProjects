#include "hashTable2.h"
#include "functions.h"
#include "hashTable.h"

int hash2_code(uintptr_t page_address) {
  	int hash_index = page_address % SIZE2;

  	return hash_index;
}

void hash2_init(MemoryDataItem2 * hash_table[]) {
    int i = 0;
    int size = SIZE2;
    int j;

    while (i < size) {
        hash_table[i] = NULL;
        i++;
    }
}

MemoryDataItem2 * hash2_insert_address(MemoryDataItem2 * hash_table2[], MemoryDataItem * bucket, size_t size) {
    int hash_index = hash2_code(bucket->page_address);

    int j;
    MemoryDataItem2 *nextHT = (MemoryDataItem2*) malloc(sizeof (MemoryDataItem2));

    if (nextHT == NULL) {
        printf("Cannot allocate memory!\n");
        exit(0);
    }

    if (hash_table2[hash_index] != NULL) {
        hash_table2[hash_index]->previous = nextHT;
    }

    nextHT->pointerToMDR = bucket;
    nextHT->size = size;
    nextHT->page_address = bucket->page_address;
    nextHT->next = hash_table2[hash_index];
    nextHT->previous = NULL;
    hash_table2[hash_index] = nextHT;

    return nextHT;
}

MemoryDataItem2 * hash2_search_address(MemoryDataItem2 * hash_table2 [], uintptr_t page_address) {
    int hash_index = hash2_code(page_address);
    int i;

    MemoryDataItem2 *nextHT;
    nextHT = hash_table2[hash_index];

    while (nextHT != NULL) {
        if (nextHT->page_address == page_address) {
            return nextHT;
        }
        
        nextHT = nextHT->next;
    }
    
    return NULL;
}
