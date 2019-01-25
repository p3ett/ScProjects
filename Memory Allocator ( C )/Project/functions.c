#include <stdint.h>

#include "functions.h"
#include "MemoryStatus.h"
#include "hashTable.h"
#include "hashTable2.h"

#define TRUE 1
#define FALSE 0

static int first_time_usage = FALSE;
static MemoryStatus memoryStatus;
static MemoryDataItem * hash_table1[8];
static MemoryDataItem2 * hash_table2[SIZE2];

int myPow(int x,int n) {
    int i; 
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}

uintptr_t allocate1MB() {
    uintptr_t address = (uintptr_t) malloc(1048576);
    while (address % 4096 != 0) {
        address++;
    }
    return address;
}

size_t myround(size_t memory_amount) {

	int power = 5;

	if( memory_amount <= 32 )
		return 32;
	else
		while(true)
		{
			if( memory_amount > myPow(2,power) )
				power++;
			else
				return (myPow(2,power));			
		}
}

void * mymalloc(size_t memory_size) {
    int i;
    if (memory_size > 4096) {
        return malloc(memory_size);
    } else {
        if (first_time_usage == FALSE) {
            first_time_usage = TRUE;

            memoryStatus.counter = 0;
            memoryStatus.nextPage = allocate1MB();
            hash_init(hash_table1);
            hash2_init(hash_table2);
        }

        MemoryDataItem * bucket = hash_search_amount(hash_table1, memory_size);
        if (bucket != NULL) {
            uintptr_t address = bucket->page_address;
            size_t rounded_amount = myround(memory_size);

            for (i = 0; i < 128; i++) {
                int k = bucket->bit_vector[i / 8] & (1 << (i - (i / 8)*8));
                if (k == 0) {
                    bucket->bit_vector[i / 8] = bucket->bit_vector[i / 8] | (1 << (i - (i / 8)*8));
                    break;
                }
            }
            address = address + i*rounded_amount;

            return (void*) address;
        } else {
            if (memoryStatus.counter == 255) {
                memoryStatus.counter = 0;
                memoryStatus.nextPage = allocate1MB();
            }

            MemoryDataItem * newbucket = hash_insert_amount(hash_table1, memory_size, memoryStatus.nextPage);
            newbucket->bit_vector[0] = newbucket->bit_vector[0] | 1;

            size_t rounded_amount = myround(memory_size);
            
            hash2_insert_address(hash_table2, newbucket, rounded_amount );


            uintptr_t address = memoryStatus.nextPage;

            memoryStatus.counter++;
            memoryStatus.nextPage = memoryStatus.nextPage + 4096;

            return (void*) address;
        }
    }
}

void myfree(void * memory_address) {
    uintptr_t temp = (uintptr_t) memory_address;
    uintptr_t page_address = (temp / 4096)*4096;

    MemoryDataItem2 * bucket2 = hash2_search_address(hash_table2, page_address);

    if (bucket2 == NULL) {
        free(memory_address);
    } else {
        uintptr_t offset = temp % 4096;
        int i = offset / bucket2->size;
        
        MemoryDataItem * bucket = bucket2->pointerToMDR;
	
       	bucket->bit_vector[i / 8] = bucket->bit_vector[i / 8] & ~ (1 << (i - (i / 8)*8));
    }
}

