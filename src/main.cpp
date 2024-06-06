#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"


#include "../h/MemoryAllocator.hpp"

//#include "../h/print.h"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Scheduler.hpp"

#include "../h/List.hpp"
#include "../h/Queue.hpp"

#include "../h/TCB.hpp"
#include "../h/_Semaphore.hpp"

#include "../h/workers.hpp"

#include "../h/BoundedBuffer.hpp"

#include "../h/_Console.hpp"
#include "../h/Idle.hpp"

//testovi
#include "../test/Threads_C_API_test.hpp"
#include "../test/ThreadSleep_C_API_test.hpp"
#include "../test/printing.hpp"

#include "../h/Stoperica.hpp"


bool isFinishedOutput = false;

void outputHandler(void* arg){
    while(!isFinishedOutput){
        _Console::handlerOutput();
        thread_dispatch();
    }
}



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
extern void userMain();

bool userMainFinished = false;

void userMainWrapper(void* args){
userMain();
userMainFinished =true;
}

int main(){

    //postavljanje adrese prekidne rutine u stvec registar
     Riscv::w_stvec((uint64) &Riscv::supervisorTrap);


    MemoryAllocator::initFree();
    //MORAJU DA SE INICIJALIZUJU POSLE MEMORY ALLOCATOR-A
    Scheduler::initialize();
    _Console::initialize();




TCB* mainTCB = nullptr;
thread_create(&mainTCB,nullptr,nullptr);

 TCB::running = mainTCB;

TCB* outputTCB=nullptr;
thread_create(&outputTCB,&outputHandler,nullptr);

 //prekidi su podrazumevano dozvoljeni
Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

//postavljanje user mode-a na true
Riscv::setMode(true);


uint64 a0;
__asm__ volatile("mv %0,a0":"=r"(a0));
__asm__ volatile("li a0,0x1024");

__asm__ volatile ("ecall");

__asm__ volatile("mv a0,%0" ::"r"(a0));



TCB* userMainTCB;
thread_create(&userMainTCB,&userMainWrapper,nullptr);


while(!userMainFinished){
thread_dispatch();
}


    Riscv::setMode(false);
    __asm__ volatile("mv %0,a0":"=r"(a0));
    __asm__ volatile("li a0,0x1024");


    __asm__ volatile ("ecall");
    __asm__ volatile("mv a0,%0" ::"r"(a0));
//userMain je zavrsen;iskljucuje se user mode

while(!_Console::buffersAreEmpty()){thread_dispatch();}




isFinishedOutput = true;

Scheduler::clear();

return 0;

}