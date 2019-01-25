#ifndef MEMORYSTATUS_H
#define MEMORYSTATUS_H

typedef struct MemoryStatus {
    uintptr_t nextPage;  
    int counter;
} MemoryStatus;

#endif /* MEMORYSTATUS_H */

