//
// Created by os on 5/12/24.
//

#include "../h/Scheduler.hpp"
#include "../h/Idle.hpp"

TCB* Scheduler::mainThr = nullptr;
Queue<TCB*>* Scheduler::threads = nullptr;
//List<TCB*> * Scheduler::threads = nullptr;

TCB* Scheduler::get(){
    //return mainThr;
    TCB** t = threads->get();
    /*if(t==nullptr)
        return Idle::getIdle();*/

   // TCB** t = threads->getFrontElem();
    return *t;

}

void Scheduler::clear(){
    while(!threads->isEmpty()){
        delete get();
       //delete (*threads->getFrontElem());
    }
}

void Scheduler::put(TCB* thread){
    //if(mainThr==nullptr) mainThr=thread;
   threads->put(thread);
   // threads->addNodeToBack(thread);
}

void Scheduler::initialize(){
    threads = new Queue<TCB*>(512);
  // threads = new List<TCB*>();
}