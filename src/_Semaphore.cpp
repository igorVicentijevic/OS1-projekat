//
// Created by os on 5/20/24.
//

#include "../h/_Semaphore.hpp"
#include "../h/Scheduler.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_c.hpp"


_Semaphore::_Semaphore(int val):val(val){}

int _Semaphore::wait(){
    if(ret==-1) return -1;

    if(--val<0){


        blocked.put(TCB::running);
        /*Riscv::pushRegistersV();

        TCB* old = TCB::running;
	    TCB* t = Scheduler::get();
        TCB::running = t?t:TCB::running ;

        TCB::contextSwitch(&old->context, &TCB::running->context);

        Riscv::popRegistersV();*/
        TCB::running->putBackInScheduler = false;
        TCB::dispatch();
    }

    return 0;
}

int _Semaphore::timedWait(time_t timeout){
    time_t startTime = TCB::totalTimeSliceCounter
    ret = this->wait();

    if(ret == -1) return -1;
    //SEMDEAD -1
    //TIMEOUT -2
    if(TCB::totalTimeSliceCounter - startTime > timeout) return -2;

    return 0;
}

int _Semaphore::tryWait(){
	if(ret == -1) return -1;
	if(val-1<0) return 0;
	return 1;
}

int _Semaphore::signal(){

    if(val++<0){ //0

        TCB* t = *blocked.get();
        t->putBackInScheduler = true;
        Scheduler::put(t);
    }

    return 0;
}

_Semaphore* _Semaphore::sem_create(int val){
    return new _Semaphore(val);
}

void _Semaphore::close(){
    while(!blocked.isEmpty()){
         TCB* current = *blocked.get();
          current->putBackInScheduler=true;
        Scheduler::put(current);
    }

    ret = -1;

}


