#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdint.h>
#include "my_sched.h"
#include "cproc.h"

void parse_opt (int argc, char *argv[], my_sched_t *my_sched) {
    int c;

    static struct option long_options[] = {
        {"child_num", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int option_index = 0;

    while ((c = getopt_long(argc, argv, "c:", long_options, &option_index)) != -1) {
        switch (c) {
        case 'c':
            my_sched->cnum = atoi(optarg);
			if (my_sched->cnum > MAX_CNUM)
				my_sched->cnum = MAX_CNUM;
            break;
        default:
            abort();
        }
    }

	printf("Client Num: %d\n", my_sched->cnum);
}

int main (int argc, char **argv) {
	my_sched_t my_sched;
	cproc_t *cproc;
	int i;
	int res;

	memset(&my_sched, 0, sizeof(my_sched_t));
	parse_opt(argc, argv, &my_sched);

	sched_init(&my_sched);

	for (i = 0; i < my_sched.cnum; i++) {
		cproc = sched_make_cproc(&my_sched);
		if (cproc != NULL) { // child process
			cproc_sleep_until_exit(&my_sched, cproc);
		}
	}

	for (i = 0; i < my_sched.cnum; i++) {
		sched_ins_cproc(&my_sched, i);
	}

	while (my_sched.exit_cnum < my_sched.cnum) {
		sched_do_scheduling(&my_sched);
	}

	uint64_t total_ta_time = 0;
	uint64_t total_r_time = 0;
	int before_dl_proc = 0;
	for (i = 0; i < my_sched.cnum; i++) {
		cproc = my_sched.cprocs + i;
		cproc_print(cproc);
		total_ta_time += cproc->exit_tick;
		total_r_time += cproc->first_tick;
		if (cproc->exit_tick <= cproc->deadline)
			before_dl_proc++;
		waitpid(cproc->pid, &res, WNOHANG);
	}
	printf("Avg. Turnaround time: %ld ticks\n", total_ta_time/my_sched.cnum);
	printf("Avg. Response time: %ld ticks\n", total_r_time/my_sched.cnum);
	printf("Finised before the deadline: %.2f%%\n", (double)before_dl_proc/(double)my_sched.cnum*100);
	sched_destroy(&my_sched);
}
