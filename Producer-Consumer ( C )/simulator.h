#ifndef SIMULATOR_H
#define SIMULATOR_H

#define true 1
#define false 0

#define QUEUES 2

union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
};

typedef struct InMessage {
    pid_t pid;
    char line[500];
} InMessage;

typedef struct OutMessage {
    pid_t pid;
    char md5[50];
} OutMessage;


/* functions */
int sem_P(int sem_id, int no);
int sem_V(int sem_id, int no);

int sem_Init(int sem_id, int pos, int val);
void free_resources(int shm_id, int sem_id);


void p_main(int id, char* pmem, int sems_in, int sems_out);

void c_main(int c_id, char* pmem, int sems_in, int sems_out, int K, pid_t * p_pids, int N);



void queue_put_in(InMessage * ptr, char * pmem, int sems);

void queue_get_in(InMessage * ptr, char * pmem, int sems);

void queue_put_out(OutMessage * ptr, char * pmem, int sems);

void queue_get_out(OutMessage * ptr, char * pmem, int sems);

void queue_write(int value, char* pmem, int pos);

int queue_read(char * pmem, int pos);

void md5hash(char * src, char * dest);

#endif /* SIMULATOR_H */

