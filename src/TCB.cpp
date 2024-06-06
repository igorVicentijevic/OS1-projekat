
// Created by os on 5/12/24.
//

#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
//#include "../h/print.h"
#include "../h/Riscv.hpp"
#include "../h/Idle.hpp"
#include "../h/syscall_c.hpp"
#include "../h/kalloc.h"

TCB* TCB::running = nullptr;
uint64 TCB::ID=0;
uint64 TCB::timeSliceCounter = 0;
uint64 TCB::totalTimeSliceCounter = 0;



List<TCB*>* TCB::sleepingThreads = nullptr;



void TCB::start(){
    if(!body) return;
    if(hasStarted) return;

//    printString("\nPokrenuta nit PID: ");
  //  printInt(pid);
 //   printString("\n");


    Scheduler::put(this);
    hasStarted=true;
}



void TCB::dispatch(){
    TCB* old = TCB::running;
    if( old->getPutBackInScheduler() && !old->isFinished()){
       Scheduler::put(old);
    }

	TCB::running = Scheduler::get();
//	TCB* t = Scheduler::get();
//    TCB::running = t?t:TCB::running ;
	//if(old==TCB::running) return;

    TCB::contextSwitch(&old->context, &TCB::running->context);
	//int volatile x = 1;

}

void TCB::removeTCBfromSleepingThreads(TCB *tcb){
    if(sleepingThreads==nullptr) return;

    time_t timeToAdd = 0;
    int i = 0;

    //trazimo tcb i kad ga nadjemo azuriramo vremena niti nakon njega
    while(i<sleepingThreads->getNumOfElements()){

         TCB* curr = *(sleepingThreads->readElemAtPos(i));
         if(curr==tcb){
            timeToAdd = curr ->timeToSleep;
            sleepingThreads->removeNodeFromPos(i);
            break;
        }
         i++;
    }

    //azuriramo ostala vremena (povecavamo za vreme koje bi cekalo tcb koje se izbacuje)
    while(i<sleepingThreads->getNumOfElements()){
         TCB* curr = *(sleepingThreads->readElemAtPos(i));
         curr->timeToSleep +=timeToAdd;
         i++;
    }


}

void TCB::updateSleepingThreadsTime(){
  if(sleepingThreads == nullptr) return;

	int n = sleepingThreads->getNumOfElements();
	int i = 0;
    if(n <= 0) return;

    TCB* first = *(sleepingThreads->readElemAtPos(0));

    first->timeToSleep--;

    while(i<sleepingThreads->getNumOfElements()){
         TCB* curr = *(sleepingThreads->readElemAtPos(i));
         if(curr->timeToSleep>0) break;

         curr->putBackInScheduler = true;

        sleepingThreads->removeNodeFromPos(i--);
		 Scheduler::put(curr);
	     //i= i-1<0 ? 0:i-1);
         //printString("Probudjena nit!\n");
         i++;
    }

	/*while(i<sleepingThreads.getNumOfElements()){
		TCB* curr = *(sleepingThreads.readElemAtPos(i));

		curr->timeToSleep--;
		if(curr->timeToSleep <= 0)
		{
			curr->putBackInScheduler = true;
			Scheduler::put(curr);
			sleepingThreads.removeNodeFromPos(i--);//i= i-1<0 ? 0:i-1);

			//n--;
			printString("Probudjena nit!\n");

		}
        i++;
	}*/




}

void TCB::putTCBToSleep(TCB* thrToSleep){

  if(sleepingThreads==nullptr){
    sleepingThreads =new List<TCB*>();
    }

	thrToSleep -> putBackInScheduler= false;
	if(sleepingThreads->getNumOfElements() <=0){
		//nema uspavanih niti; dodaje se kao jedina u listu
		sleepingThreads->addNodeToBack(thrToSleep);

        TCB::dispatch();
		return;
	}

	//ako nit koja se ubacuje duze ceka od prve niti u listi, smanjujemo vreme kako bi gledali
//koliko nam je preostalo vremena  u odnosu na prvu nit
    TCB* first = *(sleepingThreads->readElemAtPos(0));

	if(thrToSleep -> timeToSleep < first->timeToSleep)
	{
        first->timeToSleep -= thrToSleep->timeToSleep;

        sleepingThreads->addNodeToPos(0,thrToSleep);

         TCB::dispatch();
        return;
    }

	thrToSleep-> timeToSleep -= first->timeToSleep;

	// trazi se nit koja ceka duze od niti koja se ubacuje i ispred nje se ulancava nit koja se uspavljuje
	for(int i = 1; i<sleepingThreads->getNumOfElements(); i++){
		TCB* curr = *(sleepingThreads->readElemAtPos(0));

		if(curr->timeToSleep>=thrToSleep->timeToSleep){
			sleepingThreads->addNodeToPos(i,thrToSleep);

             TCB::dispatch();

			return;
		}
	}

	sleepingThreads->addNodeToBack(thrToSleep);
     TCB::dispatch();
}

void TCB::yield(){
         //poziva se iz prekidne rutine, jer se prilikom ulaska i izlaska iz p. rutine cuvaju, odnosno uzimaju registri sa steka
       Riscv::w_a0(0x13);
    __asm__ volatile("ecall");//promena konteksta iz prekidne rutine
}

TCB* TCB::createThread(Body body, void* args, void* stack_space, uint64 timeSlice){
    //stack_space = new uint64[DEFAULT_STACK_SIZE];
    return new TCB(body,args,stack_space,timeSlice);
}


void TCB::threadWrapper(){
    Riscv::popSppSpie();
    TCB::running->body(TCB::running->args);
    TCB::running->setFinished(true);
    TCB::yield();
}



