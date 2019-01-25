#include "hashTable.h"
#include "functions.h"

int hash_code(size_t memory_amount) {
	int power = 5;
	int hash_index = 0;

	if( memory_amount <= 32 )
		return hash_index;
	else
		while(true)
		{
			if( memory_amount > myPow(2,power) )
			{
				power++;
				hash_index++;
			}
			else
				return hash_index;			
		}
}

void hash_init(MemoryDataItem * hash_table[]) {
    int i = 0;
    int size = SIZE1;
    int j;

    while (i < size) {
        hash_table[i] = NULL;
        i++;
    }
}

MemoryDataItem * hash_insert_amount(MemoryDataItem * hash_table[], size_t memory_amount, uintptr_t page_address) {
    int hash_index = hash_code(memory_amount);
    int j;
    MemoryDataItem *nextHT = (MemoryDataItem*) malloc(sizeof (MemoryDataItem));
	
    if (nextHT == NULL) {
        printf("Cannot allocate memory!\n");
        exit(0);
    }

    if (hash_table[hash_index] != NULL) {
        hash_table[hash_index]->previous = nextHT;
    }
    
    nextHT->page_address = page_address;
    nextHT->next = hash_table[hash_index];
    nextHT->previous = NULL;
    hash_table[hash_index] = nextHT;

    for (j = 0; j < 16; j++) {
        nextHT->bit_vector[j] = 0;
    }

    return nextHT;
}

MemoryDataItem * hash_search_amount(MemoryDataItem * hash_table[], size_t memory_amount) {
    int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    int hash_index = hash_code(memory_amount);
    int bits_to_check = bits[hash_index];
    int i;

    MemoryDataItem *nextHT;
    nextHT = hash_table[hash_index];

    while (nextHT != NULL) {
        for (i = 0; i < bits_to_check; i++) {
            int k = nextHT->bit_vector[i / 8] & (1 << (i - (i / 8)*8));

            // move nextHT to the beginning of list
            if (k == 0) {
                return nextHT;
            }
        }
        nextHT = nextHT->next;
    }
    
    return NULL;
}
