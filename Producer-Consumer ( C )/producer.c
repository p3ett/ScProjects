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

int pid_match = 0;
int p_id = 0;
char * _pmem;

void exit_p(int signo) {
    printf("=========> P_%d: Completed with pid_match = %d  \n", p_id, pid_match);
    queue_write(pid_match, _pmem, p_id);
    exit(0);
}

void p_main(int id, char* pmem, int sems_in, int sems_out) {

    int lines = 0;
    int i;
    p_id = id;
    _pmem = pmem;

    InMessage a;
    OutMessage b;
    srand(getpid());

    a.pid = getpid();

    signal(SIGINT, exit_p);

    FILE * fp = fopen("shakespeare.txt", "r");

    while (fp) {
        char c = fgetc(fp);
        if (c == EOF) {
            break;
        }
        if (c == '\n') {
            lines++;
        }
    }

    while (true) {
        // read a random line from file
        int lineno = rand() % lines;

		// take the cursor back to the start
        rewind(fp);

        for (i = 0; i < lineno; i++) {
            fgets(a.line, sizeof(a.line), fp);
        }

        printf("Producer_%d: Line %d copied successfully\n", p_id, lineno);
        printf("Producer_%d: Waiting for queue to become available \n", p_id);
        queue_put_in(&a, pmem, sems_in);
        printf("Producer_%d: Line %d placed to queue successfully\n", p_id, lineno);

        queue_get_out(&b, pmem, sems_out);
        printf("P_%d: Consumed: %d: %s\n", p_id, b.pid, b.md5);

        if (b.pid == getpid()) {
            pid_match++;
        }
    }
}
