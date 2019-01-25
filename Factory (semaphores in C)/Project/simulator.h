
#ifndef SIMULATOR_H
#define SIMULATOR_H

/* Union semun */
union semun {
    int val; /* value for SETVAL */
    struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
};

typedef struct Accessory {
    char id[5];
    int type;
    struct timeval t0;
} Accessory;

#define true 1
#define false 0

#define QUEUES 5

/* functions */
int sem_P(int sem_id, int no);
int sem_V(int sem_id, int no);

int sem_Init(int sem_id, int pos, int val);
void free_resources(int shm_id, int sem_id);



void construction_main(int construct_id, char * pmem_paint, int sems_paint, int Y);

void paint_main(char * pmem_paint, int sems_paint, char * pmem_check1, int sems_1, char * pmem_check2, int sems_2, char * pmem_check3, int sems_3, int Y);

void check_main(int check_id, char * pmem_check, int sems_check, int sems_sunch, char * pmem_assembly, int sems_assembly, int Y);

void assembly_main(char * pmem_assembly, int sems_assembly, int sems_sunch, int Y);


void queue_put(Accessory * ptr, char * pmem, int sems);

void queue_get(Accessory * ptr, char * pmem, int sems);


#endif /* SIMULATOR_H */

