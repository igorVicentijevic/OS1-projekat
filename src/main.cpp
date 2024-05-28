#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"


#include "../h/MemoryAllocator.hpp"

#include "../h/print.h"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Scheduler.hpp"

#include "../h/List.hpp"
#include "../h/Queue.hpp"

#include "../h/TCB.hpp"
#include "../h/_Semaphore.hpp"

#include "../h/workers.hpp"

#include "../h/BoundedBuffer.hpp"

#include "../h/Console.hpp"
#include "../h/Idle.hpp"

//testovi
#include "../Test/Threads_C_API_test.hpp"
#include "../Test/ThreadSleep_C_API_test.hpp"
#include "../Test/printing.hpp"





void stoperica(void* arg){
    Semaphore* sem = ((Semaphore*)arg);


    for(int i = 1; i < 30; i++){
        printInt(i);
        printString("\n");
        //time_sleep(1);

        Thread::sleep(5);
    }

    for(int i = 30; i < 50; i++){
        printInt(i);
        printString("\n");
       // time_sleep(1);
    }

 for(int i = 50; i < 700; i++){
        printInt(i);
        printString("\n");
        //time_sleep(1);
    }

    printString("Zavrseno brojanje!\n");
    sem->signal();
}
//extern void userMain();

bool userMainFinished = false;
void userMainWrapper(void* args){
//userMain();
userMainFinished =true;
}

int main(){

    //Riscv::w_stvec((uint64) &Riscv::stvecVectorTable | 0b01);
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
   // Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //Riscv::ms_sstatus(0x2);

    //MORAJU DA SE INICIJALIZUJU POSLE MEMORY ALLOCATOR-A
    Scheduler::initialize();
    Console::initialize();


TCB* mainTCB;
thread_create(&mainTCB,nullptr,nullptr);
    TCB::running = mainTCB;


TCB* userMainTCB;
thread_create(&userMainTCB,&userMainWrapper,nullptr);

//Idle::getIdle();
//

//Idle::getIdle();

    //threads[0] = TCB::createThread(nullptr,nullptr); //main


 //prekidi su podrazumevano dozvoljeni
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    //printString("Dozvoljeni prekidi");
    //prekidi su podrazumevano dozvoljeni
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    //MemoryAllocator::initFree();




   /* char* testAlloc =(char*) MemoryAllocator::memAlloc(512);
    testAlloc[0]='a';
    testAlloc[1]='b';
    printString(testAlloc);
   /* char* testAlloc2 = (char*) MemoryAllocator::memAlloc(1024);
    for(int i = 0; i < 5; i++)
        testAlloc2[i] = 'z'-i;
    testAlloc2[6] = '\n';
    testAlloc2[7] = '\0';
    printString(testAlloc2);
    MemoryAllocator::free(testAlloc);
    printInt(MemoryAllocator::getNumOfFreeBlocks());

	printString("\nNovi deo\n");
	//Riscv::handleSupervisorTrap();

    void* ptr = mem_alloc(1024);

    printInt(*(int*)ptr);
    printString("\n");

    printInt(MemoryAllocator::getNumOfFreeBlocks());


    mem_free(ptr);
    printInt(MemoryAllocator::getNumOfFreeBlocks());

   // printString("Testiramo string!\n");
   // printInt(1234);

    printString("\nTestiranje liste\n");
    List<int> l;
    l.addNodeToBack(127);
    l.addNodeToBack(128);
    l.addNodeToPos(0,129);
    printInt(*(l.readElemAtPos(0)));
    l.removeNodeFromBack();
    l.removeNodeFromPos(0);
    printInt(l.getNumOfElements());
    printInt(*(l.readElemAtPos(0)));

    printString("\nTestiranje reda\n");

    Queue<int> q;
    q.put(2);
    q.put(3);

    printInt(*(q.get()));
    printInt(*(q.get()));
    printInt(q.isEmpty());*/

  //printString("\nTestiranje Thread-ova\n");


 /* TCB* threads[3];

    //threads[0] = TCB::createThread(nullptr,nullptr); //main
	//thread_create(&threads[0],nullptr,nullptr);
    //TCB::running = threads[0];
   //threads[0]->start();
   _Semaphore* mutex = _Semaphore::sem_create(1);

   thread_create(&threads[1],workerBodyA,(void*) mutex);
	thread_create(&threads[2],workerBodyB,(void*) mutex);



    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    for(auto thread: threads) thread->start();


   while(!threads[1]->isFinished() || !threads[2]->isFinished())TCB::yield();*/



 /*   TCB* stopericaThr;
     thread_create(&stopericaThr,&stoperica,nullptr);
    while(!stopericaThr->isFinished()) thread_dispatch();

printString("Unesi karakter\n");

char c = get_c();
printStringNjihov("STRING TEST\n");

//__putc(c);
//char g = get_c();
//char h = get_c();


testSleeping();


//Testiranje konzole
/*for(int i = 0; i < 100; i++)g
put_c('a');
put_c('\n');
}

//printInt(TCB::totalTimeSliceCounter);


put_c('K');




put_c('B');*/

//printString("\nKraj!\n");*/



Semaphore testSem(0);

Thread testThread(stoperica,&testSem);

testSem.wait();

Threads_C_API_test();

if(!userMainFinished){
thread_dispatch();
}

while(!Console::buffersAreEmpty());

return 0;

}