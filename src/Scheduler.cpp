//
// Created by os on 5/12/24.
//

#include "../h/Scheduler.hpp"
#include "../h/Idle.hpp"

TCB* Scheduler::mainThr = nullptr;
Queue<TCB*>* Scheduler::threads = nullptr;


TCB* Scheduler::get(){
    //return mainThr;
    TCB** t = threads->get();
    if(t==nullptr)
        return Idle::getIdle();

    return *t;

}

void Scheduler::put(TCB* thread){
    //if(mainThr==nullptr) mainThr=thread;
    threads->put(thread);
}

void Scheduler::initialize(){
    threads = new Queue<TCB*>();
}