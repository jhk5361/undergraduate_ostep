#ifndef __CPROC_H__
#define __CPROC_H__

#include "my_cpu.h"

void cproc_sleep_until_exit (my_sched_t *my_sched, cproc_t *cproc);

static inline void cproc_print (cproc_t *cproc) {
	printf("cid: %d, pid: %d, life_time: %d, sched_time: %d, deadline: %d, exit_tick: %d, first_tick: %d\n", cproc->cid, cproc->pid, cproc->life_time, cproc->sched_time, cproc->deadline, cproc->exit_tick, cproc->first_tick);
}

#endif
