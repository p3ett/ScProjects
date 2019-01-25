/* Semaphore P - down operation, using semop */

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "simulator.h"

// https://stackoverflow.com/questions/1858050/how-do-i-compare-two-timestamps-in-c

#define timerlessthan(a, b)            \
  (((a).tv_sec == (b).tv_sec) ?            \
   ((a).tv_usec < (b).tv_usec) :            \
   ((a).tv_sec < (b).tv_sec))


#define tomicro(a) ((a).tv_sec*1000000.0 + (a).tv_usec)

/* Semaphore P - down operation , semaphore commited*/
int sem_P(int sem_id, int no) {

    struct sembuf sem_d;

    sem_d.sem_num = no;
    sem_d.sem_op = -1;
    sem_d.sem_flg = 0;

    if (semop(sem_id, &sem_d, 1) == -1) {
        perror("***** Semaphore down (P) operation *****");
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
        perror("***** Semaphore up (V) operation *****");
        return -1;
    }

    return 0;
}

/* Semaphore Init - set a semaphore’s value to val */
int sem_Init(int sem_id, int pos, int val) {

    union semun arg;

    arg.val = val;

    if (semctl(sem_id, pos, SETVAL, arg) == -1) {
        perror("***** Semaphore setting value *****");
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

void queue_put(Accessory* ptr, char* pmem, int sems) {

    sem_P(sems, 0);

    memcpy((void*) pmem, (void*) ptr, sizeof (Accessory));

    sem_V(sems, 1);
}

void queue_get(Accessory* ptr, char* pmem, int sems) {

    sem_P(sems, 1);

    memcpy((void*) ptr, (void*) pmem, sizeof (Accessory));

    sem_V(sems, 0);
}

void construction_main(int construct_id, char* pmem_paint, int sems_paint, int Y) {

    Accessory a;
    srand(getpid());

    struct timeval t0;
    struct timeval t1;

    double tall = 0;

    int copyY = Y;

    while (Y > 0) {

        usleep(rand() % 3 + 1);

        sprintf(a.id, "%04d", rand() % 10000);
        a.type = construct_id;
        gettimeofday(&a.t0, 0);

        printf("ConstructionSite_%d: Product %s/%d constructed successfully\n", construct_id, a.id, a.type);

        printf("ConstructionSite_%d: Waiting to import product %s\n", construct_id, a.id);
       
        gettimeofday(&t0, 0);
        queue_put(&a, pmem_paint, sems_paint);
        gettimeofday(&t1, 0);
       
        printf("ConstructionSite_%d: Product %s/%d imported successfully\n", construct_id, a.id, a.type);

        tall = tall + (tomicro(t1) - tomicro(t0));
        
        Y--;
    }

    double tavg = tall / copyY;

    printf("=========> ConstructionSite_%d: Average delay time to painting: %.2lf <=========\n", construct_id, tavg);

}

void paint_main(char* pmem_paint, int sems_paint, char* pmem_check1, int sems_1, char* pmem_check2, int sems_2, char* pmem_check3, int sems_3, int Y) {

    Accessory a;
    Y = Y * 3;



    while (Y > 0) {

        printf("PaintSite: Waiting to export product\n");
        queue_get(&a, pmem_paint, sems_paint);
        printf("PaintSite: Product %s/%d exported successfully\n", a.id, a.type);

        usleep(a.type + 1);

        printf("PaintSite: Product %s/%d painted successfully \n", a.id, a.type);

        if (a.type == 0) {

            printf("PaintSite: Waiting to import product %s/%d to checkSite_0\n", a.id, a.type);
            queue_put(&a, pmem_check1, sems_1);
            printf("PaintSite: Product %s imported successfully to checkSite_0\n", a.id);
        } else if (a.type == 1) {

            printf("PaintSite: Waiting to import product %s/%d to checkSite_1\n", a.id, a.type);
            queue_put(&a, pmem_check2, sems_2);
            printf("paintSite: Product %s imported successfully to checkSite_1\n", a.id);
        } else {

            printf("PaintSite: Waiting to import product %s/%d to checkSite_2\n", a.id, a.type);
            queue_put(&a, pmem_check3, sems_3);
            printf("PaintSite: Product %s imported successfully to checkSite_2\n", a.id);
        }

        Y--;
    }
}

void check_main(int check_id, char* pmem_check, int sems_check, int sems_synch, char* pmem_assembly, int sems_assembly, int Y) {

    Accessory a;

    while (Y > 0) {

        printf("Checksite_%d: Waiting to export product\n", check_id);
        queue_get(&a, pmem_check, sems_check);
        printf("CheckSite_%d: Product %s/%d exported successfully\n", check_id, a.id, a.type);

        usleep(3 - a.type);

        printf("CheckSite_%d: Product: %s/%d was checked successfully\n", check_id, a.id, a.type);

        sem_P(sems_synch, check_id);

        printf("CheckSite_%d: Waiting to import product %s/%d\n", check_id, a.id, a.type);
        queue_put(&a, pmem_assembly, sems_assembly);
        printf("CheckSite_%d: Product %s/%d imported successfully\n", check_id, a.id, a.type);

        Y--;
    }
}

void assembly_main(char* pmem_assembly, int sems_assembly, int sems_synch, int Y) {

    Accessory a;
    Accessory b;
    Accessory c;

    struct timeval completion_time;

    double tall = 0;
    
    int copyY = Y;

    while (Y > 0) {

        printf("AssemblySite: Waiting to export product\n");
        queue_get(&a, pmem_assembly, sems_assembly);
        printf("AssemblySite: Product %s/%d exported successfully\n", a.id, a.type);

        printf("AssemblySite: Waiting to export product\n");
        queue_get(&b, pmem_assembly, sems_assembly);
        printf("AssemblySite: Product %s/%d exported successfully\n", b.id, b.type);

        printf("AssemblySite: Waiting to export product\n");
        queue_get(&c, pmem_assembly, sems_assembly);
        printf("AssemblySite: Product %s/%d exported successfully\n", c.id, c.type);

        usleep(5);

        gettimeofday(&completion_time, 0);

        double t;

        if (timerlessthan(a.t0, b.t0)) {
            if (timerlessthan(a.t0, c.t0)) {
                // compare with a
                double ta = tomicro(a.t0);
                double ts = tomicro(completion_time);
                t = ts - ta;
            } else {
                // compare with c
                double tc = tomicro(c.t0);
                double ts = tomicro(completion_time);
                t = ts - tc;
            }
        } else {
            if (timerlessthan(b.t0, c.t0)) {
                // compare with b
                double tb = tomicro(b.t0);
                double ts = tomicro(completion_time);
                t = ts - tb;
            } else {
                // compare with c
                double tc = tomicro(c.t0);
                double ts = tomicro(completion_time);
                t = ts - tc;
            }
        }

        tall = tall + t;

        printf("***** Product completed with ids/type: %s/%d, %s/%d, %s/%d in %.2lf *****\n", a.id, a.type, b.id, b.type, c.id, c.type, t);

        if (a.type == b.type || a.type == c.type || b.type == c.type) {
            printf("************ (( factory exploded )) ************\n");
        }
        
        Y--;

        sem_V(sems_synch, 0);
        sem_V(sems_synch, 1);
        sem_V(sems_synch, 2);

    }

    tall = tall / copyY;

    printf("*************************************************************\n");
    printf("**** Production completed with mean service time: %.2lf ****\n", tall);
    printf("*************************************************************\n");

}

