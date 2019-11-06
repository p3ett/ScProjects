#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <openssl/md5.h>

#include "simulator.h"

/* Semaphore P - down operation , semaphore commited*/
int sem_P(int sem_id, int no) {

    struct sembuf sem_d;

    sem_d.sem_num = no;
    sem_d.sem_op = -1;
    sem_d.sem_flg = 0;

    if (semop(sem_id, &sem_d, 1) == -1) {
        perror(" **************** Semaphore down (P) operation **************");
        return -1;
    }

    return 0;
}

/* Semaphore V - up operation, semaphore released */
int sem_V(int sem_id, int no) {

    struct sembuf sem_d;

    sem_d.sem_num = no;
    sem_d.sem_op = 1;
    sem_d.sem_flg = 0;

    if (semop(sem_id, &sem_d, 1) == -1) {
        perror("**************Semaphore up (V) operation **************");
        return -1;
    }

    return 0;
}

/* Semaphore Init - set a semaphore’s value to val */
int sem_Init(int sem_id, int pos, int val) {

    union semun arg;

    arg.val = val;

    if (semctl(sem_id, pos, SETVAL, arg) == -1) {
        perror("************** Semaphore setting value **************");
        return -1;
    }

    return 0;
}

void free_resources(int shm_id, int sem_id) {

    /* Delete the shared memory segment */
    if (shm_id != 0)
        shmctl(shm_id, IPC_RMID, NULL);

    /* Delete the semaphore */
    if (sem_id != 0)
        semctl(sem_id, 0, IPC_RMID, 0);
}

void queue_write(int value, char* pmem, int pos) {
    pmem = pmem + sizeof (InMessage) + sizeof (OutMessage) + pos * sizeof (int);

    memcpy((void*) pmem, (void*) &value, sizeof (value));
}

int queue_read(char * pmem, int pos) {
    int value;
    pmem = pmem + sizeof (InMessage) + sizeof (OutMessage) + pos * sizeof (int);

    memcpy((void*) &value, (void*) pmem, sizeof (value));

    return value;
}

void queue_put_in(InMessage* ptr, char* pmem, int sems) {

    sem_P(sems, 0);

    memcpy((void*) pmem, (void*) ptr, sizeof (InMessage));

    sem_V(sems, 1);
}

void queue_get_in(InMessage* ptr, char* pmem, int sems) {

    sem_P(sems, 1);

    memcpy((void*) ptr, (void*) pmem, sizeof (InMessage));

    sem_V(sems, 0);
}

void queue_put_out(OutMessage* ptr, char* pmem, int sems) {

    pmem = pmem + sizeof (InMessage);

    sem_P(sems, 0);

    memcpy((void*) pmem, (void*) ptr, sizeof (OutMessage));

    sem_V(sems, 1);
}

void queue_get_out(OutMessage* ptr, char* pmem, int sems) {

    pmem = pmem + sizeof (InMessage);

    sem_P(sems, 1);

    memcpy((void*) ptr, (void*) pmem, sizeof (OutMessage));

    sem_V(sems, 0);
}

void md5hash(char * src, char * dest) {

    int i;
    MD5_CTX context;
    unsigned char digest[16];
    unsigned int len = strlen(src);

    MD5_Init(&context);
    MD5_Update(&context, src, len);
    MD5_Final(digest, &context);

    strcpy(dest, "");

    char buffer[3];

    for (i = 0; i < 16; i++) {
        sprintf(buffer, "%02x", digest[i]);
        strcat(dest, buffer);
    }
}
