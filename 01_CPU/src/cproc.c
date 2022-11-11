#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "cproc.h"

int cproc_update_state (my_sched_t *my_sched, cproc_t *cproc) {
	if (cproc->sched_time == 0) {
		return -1;
	}
	cproc->life_time -= cproc->sched_time;
	cproc->sched_time = 0;
	return 0;
}

void cproc_sleep_until_exit (my_sched_t *my_sched, cproc_t *cproc) {
	while(1) { 
		sem_post(&cproc->empty);
		sem_wait(&cproc->scheduled);
		cproc_update_state(my_sched, cproc);
		// sleep again, zzz..
		if (cproc->life_time == 0)
			break;
		sem_post(&cproc->empty);
	}
	sem_post(&cproc->empty);
	exit(0);
}
