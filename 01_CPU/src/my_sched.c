#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include "my_sched.h"
#include "cproc.h"
#include "list.h"

my_sched_t *sched_init (my_sched_t *my_sched) {
	int shm_id;

	shm_id = shmget(IPC_PRIVATE, sizeof(cproc_t) * my_sched->cnum, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	if (shm_id == -1)
		return NULL;

	my_sched->cprocs = (cproc_t *)shmat(shm_id, NULL, 0);
	if (my_sched->cprocs == (cproc_t *)-1)
		return NULL;

	my_sched->shm_id = shm_id;
	my_sched->clist = list_init();

	return my_sched;
}

int sched_destroy (my_sched_t *my_sched) {
	shmdt(my_sched->cprocs);
	shmctl(my_sched->shm_id, IPC_RMID, NULL);
	list_free(my_sched->clist);
	return 0;
}

// return: parent -> NULL, child -> cproc
cproc_t *sched_make_cproc (my_sched_t *my_sched) {
	pid_t pid;
	cproc_t *cproc;

	cproc = my_sched->cprocs + my_sched->cur_cnum;
	cproc->cid = my_sched->cur_cnum++;
	cproc->life_time = rand()%MAX_LIFE + 1;
	cproc->deadline = cproc->life_time + rand()%MAX_DL + 1;
	cproc->sched_time = 0;
	sem_init(&cproc->empty, 1, 0);
	sem_init(&cproc->scheduled, 1, 0);
	pid = fork();
	if (pid != 0) {
		cproc->pid = pid;
		return NULL;
	}

	return cproc;
}

cproc_t *sched_ins_cproc (my_sched_t *my_sched, int cid) {
	cproc_t *cproc = my_sched->cprocs + cid;
	cproc->lnode = list_insert(my_sched->clist, cproc);

	return cproc;
}

cproc_t *sched_del_cproc (my_sched_t *my_sched, int cid) {
	cproc_t *cproc = my_sched->cprocs + cid;
	list_delete_node(my_sched->clist, cproc->lnode);

	return cproc;
}

static cproc_t *sched_pick_cproc (my_sched_t *my_sched) {
	// do your job
	cproc_t *cproc = (cproc_t *)my_sched->clist->head->data;
	return cproc;
}

static int sched_set_sched_time (my_sched_t *my_sched, cproc_t *cproc) {
	int sched_time;
	// do your job
	// you have to set the 'cproc->sched_time'
	sched_time = cproc->life_time;
	sched_time = cproc->life_time < 20 ? cproc->life_time: 20;
	//
	cproc->sched_time = sched_time;
	cproc->done = 0;
	return sched_time;
}

static void sched_update_cproc (my_sched_t *my_sched, cproc_t *cproc) {
	// do your job
	if (cproc->life_time == 0) {
		sched_del_cproc(my_sched, cproc->cid);
		my_sched->exit_cnum++;
	} else {
		list_move_to_tail(my_sched->clist, cproc->lnode);
	}

}

static void sched_update_state (my_sched_t *my_sched, cproc_t *cproc, int sched_time) {
	my_sched->cur_tick += sched_time;
	if (cproc->first_tick == 0)
		cproc->first_tick = my_sched->cur_tick- sched_time;
	if (cproc->life_time == 0)
		cproc->exit_tick = my_sched->cur_tick;

}

// FIFO
int sched_do_scheduling (my_sched_t *my_sched) {
	int sched_time;
	cproc_t *cproc;

	cproc = sched_pick_cproc(my_sched);
	sem_wait(&cproc->empty);
	sched_time = sched_set_sched_time(my_sched, cproc);
	sem_post(&cproc->scheduled);
	sem_wait(&cproc->empty);
	sched_update_cproc(my_sched, cproc);
	sched_update_state(my_sched, cproc, sched_time);

	return 0;
}
