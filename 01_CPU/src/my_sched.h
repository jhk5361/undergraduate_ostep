#ifndef __MY_SCHED_H__
#define __MY_SCHED_H__

#include "my_cpu.h"

my_sched_t *sched_init (my_sched_t *my_sched);
cproc_t *sched_make_cproc (my_sched_t *my_sched);
cproc_t *sched_ins_cproc (my_sched_t *my_sched, int cid);
cproc_t *sched_del_cproc (my_sched_t *my_sched, int cid);
int sched_do_scheduling (my_sched_t *my_sched);
int sched_destroy (my_sched_t *my_sched);


#endif
