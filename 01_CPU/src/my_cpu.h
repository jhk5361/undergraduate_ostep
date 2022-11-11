#ifndef __MY_CPU_H__
#define __MY_CPU_H__

#define MAX_LIFE 10000
#define MAX_DL 200000
#define MAX_CNUM 1000

#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include "list.h"

typedef struct child_process {
	int cid; // child index
	int pid; // process id
	int life_time; // remained excution time
	int sched_time; // a given time for execution
	int deadline;

	// below vars are for the performance measurement
	int exit_tick; // when the process exits
	int first_tick; // when the process wakes up for the first time
	li_node *lnode; // list node
	sem_t empty;
	sem_t scheduled;
	int done;

} cproc_t;

typedef struct aegidle_sched {
	int cur_tick; // current tick
	int cnum; // the number of child processes
	int cur_cnum; // live processes
	int exit_cnum; // the number of exited proccesses
	list *clist; // scheduling list
	//pthread_mutex_t lock;
	//pthread_cond_t cond;
	int shm_id;
	cproc_t *cprocs; // array of cproc
} my_sched_t;

#endif
