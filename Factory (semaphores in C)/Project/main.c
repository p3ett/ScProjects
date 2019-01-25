#include <stdio.h>
#include <stdlib.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#include "simulator.h"

int main(int argc, char** argv) {

    int i, j;
    int Y = -1;

    int shm_id[QUEUES];
    int sem_id[QUEUES + 1];
    char* sh[QUEUES];

    pid_t pid_construction[3];
    pid_t pid_paint;
    pid_t pid_check[3];
    pid_t pid_assembly;
    pid_t pid_terminated;

    if (argc != 2) {
    
        printf("Invalid arguments. \n");
        exit(EXIT_FAILURE);
    }
    else if (atoi(argv[1]) > 0)
        Y = atoi(argv[1]);
    else {
        printf("Y must be a number greater than zero. \n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < QUEUES; i++) {
        if (i == 1 || i == 2 || i == 3) {
            /* Create shared memory and attach shared object */
            shm_id[i] = shmget(IPC_PRIVATE, sizeof (Accessory) * Y, IPC_CREAT | 0660);
            sh[i] = (char*) shmat(shm_id[i], NULL, 0);

            /* Create semaphores and initialize */
            sem_id[i] = semget(IPC_PRIVATE, 2, IPC_CREAT | 0660);
            sem_Init(sem_id[i], 0, Y); // empty
            sem_Init(sem_id[i], 1, 0); // full
        } else {
            /* Create shared memory and attach shared object */
            shm_id[i] = shmget(IPC_PRIVATE, sizeof (Accessory), IPC_CREAT | 0660);
            sh[i] = (char*) shmat(shm_id[i], NULL, 0);

            /* Create semaphores and initialize */
            sem_id[i] = semget(IPC_PRIVATE, 2, IPC_CREAT | 0660);
            sem_Init(sem_id[i], 0, 1);
            sem_Init(sem_id[i], 1, 0);
        }
    }

    sem_id[QUEUES] = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660);
    sem_Init(sem_id[i], 0, 1);
    sem_Init(sem_id[i], 1, 1);
    sem_Init(sem_id[i], 2, 1);
    

    /* Check for errors */
    for (i = 0; i < QUEUES; i++) {

        if (shm_id[i] < 0 || sem_id[i] < 0) {
            printf("Error on shared memory or semaphore.\n");

            for (j = 0; j < i; j++)
                free_resources(shm_id[j], sem_id[j]);

            exit(EXIT_FAILURE);
        }
    }

    /* Create construction sites */
    for (i = 0; i < 3; i++) {

        if ((pid_construction[i] = fork()) == -1) {
            perror("Fork");
            exit(EXIT_FAILURE);
        }

        if (pid_construction[i] == 0) {

            //printf("Create constructionSite_%d successfully\n", i);

            construction_main(i, sh[0], sem_id[0], Y);
            exit(EXIT_SUCCESS);
        }
    }

    /* Create paint site */
    if ((pid_paint = fork()) == -1) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid_paint == 0) {

        //printf("Create paintSite successfully\n");

        paint_main(sh[0], sem_id[0], sh[1], sem_id[1], sh[2], sem_id[2], sh[3], sem_id[3], Y);
        exit(EXIT_SUCCESS);
    }

    /* Create check site */
    for (i = 0; i < 3; i++) {

        if ((pid_check[i] = fork()) == -1) {
            perror("Fork");
            exit(EXIT_FAILURE);
        }

        if (pid_check[i] == 0) {

            //printf("Create checkSite_%d successfully\n", i);

            check_main(i, sh[1 + i], sem_id[1 + i], sem_id[QUEUES], sh[4], sem_id[4], Y);
            exit(EXIT_SUCCESS);
        }
    }

    /* Create assembly site */
    if ((pid_assembly = fork()) == -1) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid_assembly == 0) {

        //printf("Create assemblySite successfully\n");

        assembly_main(sh[4], sem_id[4], sem_id[QUEUES], Y);
        exit(EXIT_SUCCESS);
    }

    /* Wait for all sites to terminate */
    while ((pid_terminated = wait(NULL)) > 0);

    /* Detaach shared memory */
    for (i = 0; i < QUEUES; i++)
        shmdt(sh[i]);

    /* Delete shared memory and semaphores */
    for (i = 0; i < QUEUES; i++)
        free_resources(shm_id[i], sem_id[i]);
    
    free_resources(0, sem_id[QUEUES]);

    return EXIT_SUCCESS;
}

