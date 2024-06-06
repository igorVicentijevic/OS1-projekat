//
// Created by os on 5/25/24.
//

#include "../h/Idle.hpp"
#include "../h/print.h"
#include "../h/syscall_c.hpp"
TCB* Idle::idle = nullptr;

void Idle::idleBody(void* args)
{
    while(1){
        thread_dispatch();
    }
}

TCB* Idle::getIdle(){
    if(idle == nullptr)
    {
     thread_create(&idle,&Idle::idleBody,nullptr);
    idle->start();}
    return idle;
}