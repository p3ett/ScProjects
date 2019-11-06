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

void c_main(int c_id, char* pmem, int sems_in, int sems_out, int K, pid_t * p_pids, int N) {

    InMessage a;
    OutMessage b;
    int i = 0;

    while (K > 0) {

        printf("Consumer: Waiting for a line \n");
        queue_get_in(&a, pmem, sems_in);
        printf("Consumer: Line received successfully\n");

        b.pid = a.pid;

        md5hash(a.line, b.md5);

		printf("Consumer: Waiting for queue to become available\n");
        queue_put_out(&b, pmem, sems_out);
		printf("Consumer: Line placed to queue successfully\n");

        K--;
    }

    usleep(1000); // just to eliminate the chance of killing the last producer before he consume

    for (i = 0; i < N; i++) {
        kill(p_pids[i], SIGINT);
    }
}
