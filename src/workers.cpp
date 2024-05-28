//
// Created by os on 5/14/24.
//

#include "../h/workers.hpp"
#include "../h/TCB.hpp"
#include "../h/print.h"
#include "../h/syscall_c.hpp"
#include "../h/_Semaphore.hpp"


static uint64 fibonacci(uint64 n){
    if(n<=1) return n;
    if(n%4==0) TCB::yield();
    return fibonacci(n-1)+fibonacci(n-2);
}
void workerBodyA(void* arg)
{
    _Semaphore* mutex = (_Semaphore*) arg;
	mutex->wait();
    //Thread::yield();
    uint8 i = 0;
    for (; i < 3; i++)
    {
        printString("A: i=");
        printInt(i);
        printString("\n");
    }

    printString("A: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("A: t1=");
    printInt(t1);
    printString("\n");



    uint64 result = fibonacci(7);//20);
    printString("A: fibonaci=");
    printInt(result);
    printString("\n");

    for (; i < 6; i++)
    {
        printString("A: i=");
        printInt(i);
        printString("\n");
    }

	mutex->signal();

   // TCB::running->setFinished(true);
    //TCB::yield();
}

void workerBodyB(void* arg)
{
	_Semaphore* mutex = (_Semaphore*) arg;
	mutex->wait();
    //Thread::yield();
    uint8 i = 10;

    for (; i < 13; i++)
    {
        printString("B: i=");
        printInt(i);
        printString("\n");
    }

    printString("B: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();


    uint64 result = fibonacci(17);
    printString("B: fibonaci=");
    printInt(result);
    printString("\n");

    for (; i < 16; i++)
    {
        printString("B: i=");
        printInt(i);
        printString("\n");
    }

	mutex->signal();
 //  TCB::running->setFinished(true);
   //TCB::yield();
}

void testA(void* arg){
	//while(true) Thread::yield();
 TCB::running->setFinished(true);
   TCB::yield();
	return;
}

void testB(void* arg){
//while(true) Thread::yield();
 TCB::running->setFinished(true);
   TCB::yield();
	return;
}
