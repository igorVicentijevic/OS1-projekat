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
         blocked.addNodeToBack(TCB::running);
        TCB::running->putBackInScheduler = false;
        TCB::dispatch();
    }

    return 0;
}

int _Semaphore::timedWait(time_t timeout){

    if(ret==-1) return -1;

    int retVal = 0;
    if(--val<0){

        blocked.addNodeToBack(TCB::running);
        //u listu niti koje spavaju na semaforu dodajemo tekucu nit
       // sleeping.addNodeToBack(TCB::running);

        TCB::running->timeToSleep = timeout;
       TCB::putTCBToSleep(TCB::running);

        if(TCB::running -> timeToSleep > 0)
        {


            //probudjena je zbog signala
            TCB::removeTCBfromSleepingThreads(TCB::running);

            //return 0;
             retVal = 0;
        }
        else{

            //probudjena je jer je istekao timeout
            blocked.removeGivenElem(TCB::running);

            val++;
           retVal = -2;

        }
        //izbacujemo iz niti koje spavaju na semaforu tekucu nit
      //  sleeping.removeGivenElem(TCB::running);
    }

    return ret;
}
/*
int _Semaphore::timedWait(time_t timeout){
    time_t startTime = TCB::totalTimeSliceCounter;

    ret = this->tryWait();
    if(ret == 1){ // semafor se zakljucava

    }

    if(ret == -1) return -1;
    //SEMDEAD -1
    //TIMEOUT -2
    if(TCB::totalTimeSliceCounter - startTime > timeout) return -2;

    return 0;
}
*/
int _Semaphore::tryWait(){
	if(ret == -1) return -1;
	if(val-1<0) return 0;
	return 1;
}

int _Semaphore::signal(){

    if(ret==-1) return ret;
    if(val++<0){

         if(blocked.isEmpty()) return 0;
        //TCB* t = *blocked.get();

        TCB* t = *(blocked.getFrontElem());
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
         //TCB* current = *blocked.get();
        TCB* current = *(blocked.getFrontElem());
          current->putBackInScheduler=true;
        Scheduler::put(current);
    }

  /*  while(!sleeping.isEmpty()){
        TCB* current = *(sleeping.getFrontElem());
        current->putBackInScheduler=true;
        Scheduler::put(current);
    }*/

    ret = -1;

}


