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

    int i, j, K, P;
    int sum_pid_match = 0;

    int shm_id[1];
    int sem_id[2];
    char* sh[1];

    pid_t * pid_p;
    pid_t pid_c;
    pid_t pid_terminated;

    if (argc != 3) {
        printf("Invalid arguments. \n");
    } else {
        P = atoi(argv[1]);
        K = atoi(argv[2]);

		if (P <= 0 || K <= 0) {
     	   printf("P and K must be a number greater than zero. \n");
      	   exit(EXIT_FAILURE);
    	}
    }

    pid_p = malloc(sizeof(pid_p) * P);

    shm_id[0] = shmget(IPC_PRIVATE, sizeof(InMessage) + sizeof(OutMessage) + P * sizeof(int), IPC_CREAT | 0660);
    sh[0] = (char*) shmat(shm_id[0], NULL, 0);

	/* Check for errors */
	 if (shm_id[0] < 0) {
        printf("Error on shared memory.\n");
        free_resources(shm_id[0], 0);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 2; i++) {
        sem_id[i] = semget(IPC_PRIVATE, 3, IPC_CREAT | 0660);
        sem_Init(sem_id[i], 0, K); // empty
        sem_Init(sem_id[i], 1, 0); // full
        sem_Init(sem_id[i], 2, 1); // mutex
    }

    /* Check for errors */
    for (i = 0; i < 2; i++) {
        if (sem_id[i] < 0) {
            printf("Error on shared semaphore.\n");

            for (j = 0; j < i; j++)
                free_resources(0, sem_id[j]);

            free_resources(shm_id[0], 0);

            exit(EXIT_FAILURE);
        }
    }

    /* Create Producers */
    for (i = 0; i < P; i++) {

        if ((pid_p[i] = fork()) == -1) {
            perror("Fork");
            exit(EXIT_FAILURE);
        }

        if (pid_p[i] == 0) {

            printf("Create Producer_%d with pid %d successfully\n", i, getpid());

            p_main(i, sh[0], sem_id[0], sem_id[1]);
            exit(EXIT_SUCCESS);
        }
    }

    /* Create Consumer */
    if ((pid_c = fork()) == -1) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid_c == 0) {

        printf("Create Consumer successfully\n");

        c_main(1, sh[0], sem_id[0], sem_id[1], K, pid_p, P);
        exit(EXIT_SUCCESS);
    }

    /* Wait for all sites to terminate */
    while ((pid_terminated = wait(NULL)) > 0);

	/* calculate the matches */
    for (i = 0; i < P; i++) {
        int m = queue_read(sh[0], i);
        sum_pid_match = sum_pid_match + m;
    }

    /* Detach shared memory */
    shmdt(sh);

    /* Delete shared memory and semaphores */
    for (i = 0; i < 2; i++)
        free_resources(0, sem_id[i]);

    free_resources(shm_id[0], 0);

    free(pid_p);


    printf("P is:  %d \n", P);
    printf("K is:  %d \n", K);
    printf("sum_pid_match is:  %d \n", sum_pid_match);

    return EXIT_SUCCESS;
}

