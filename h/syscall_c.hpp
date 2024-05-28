#ifndef syscall_c
#define syscall_c

#include "../lib/hw.h"
#include "../h/TCB.hpp"

void * mem_alloc(size_t size);

int mem_free(void* free);


typedef TCB* thread_t;

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg);

int thread_exit ();

void thread_dispatch ();

//sem
class _sem;
typedef _sem* sem_t;
int sem_open (sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int sem_timedwait(sem_t id,time_t timeout);

int sem_trywait(sem_t id);

int time_sleep(time_t sleepTime);

//CONSOLE

void put_c(char c);

char get_c();


#endif