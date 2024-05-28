//
// Created by os on 5/9/24.
//

#include "../h/syscall_c.hpp"
#include "../h/Riscv.hpp"
#include "../h/_Semaphore.hpp"


void * mem_alloc(size_t size){



    __asm__ volatile("mv a1, %0" : : "r"(size));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile("ecall");

    void* returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int mem_free(void* free){



    __asm__ volatile("mv a1, %0" : : "r"(free));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");

   int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg){
    __asm__ volatile("mv a7, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));


    __asm__ volatile("li a0, 0x11");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;


}

int thread_exit (){
    __asm__ volatile("li a0, 0x12");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

void thread_dispatch (){
	__asm__ volatile("li a0, 0x13");
__asm__ volatile("ecall");

}

// SEMAPHORE


int sem_open (sem_t* handle, unsigned init){
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));


    __asm__ volatile("li a0, 0x21");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %0" : : "r"(handle));


    __asm__ volatile("li a0, 0x22");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int sem_wait (sem_t id){

    __asm__ volatile("mv a1, %0" : : "r"(id));


    __asm__ volatile("li a0, 0x23");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int sem_signal (sem_t id){

    __asm__ volatile("mv a1, %0" : : "r"(id));


    __asm__ volatile("li a0, 0x24");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int sem_timedwait(sem_t id,time_t timeout){

    __asm__ volatile("mv a2, %0" : : "r"(timeout));
    __asm__ volatile("mv a1, %0" : : "r"(id));



    __asm__ volatile("li a0, 0x25");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int sem_trywait(sem_t id){
    __asm__ volatile("mv a1, %0" : : "r"(id));


    __asm__ volatile("li a0, 0x26");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

int time_sleep(time_t sleepTime){
    __asm__ volatile("mv a1, %0" : : "r"(sleepTime));


    __asm__ volatile("li a0, 0x31");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (int) ret;
}

//CONSOLE

void put_c(char c){
    __asm__ volatile("mv a1, %0" : : "r"((uint64)c));


    __asm__ volatile("li a0, 0x42");

    __asm__ volatile("ecall");


}

char get_c(){
    __asm__ volatile("li a0, 0x41");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return (char) ret;
}




