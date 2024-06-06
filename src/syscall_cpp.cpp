//
// Created by os on 5/28/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/_Console.hpp"
#include "../lib/hw.h"




struct Arguments{
    Thread* thisObj;
    void* args;
};

void Thread::runWrapper(void * arg){
    ((Thread*) arg)->run();
}

void Thread::threadWrapper(void * arg){
    Thread* thisObj = ((Arguments*) arg)->thisObj;
    void* args = ((Arguments*) arg)->args;

    ((Thread*) thisObj)->body(args);
}


Thread::Thread(void (*body)(void*), void* arg){
   this-> body = body;
   this->arg = arg;

   Arguments* args = new Arguments();
   args->thisObj = this;
   args->args = arg;

    thread_create(&myHandle, &threadWrapper, (void*)args );

    //delete args;
}

int Thread::start(){
    myHandle->start();
    return 0;

}

void Thread::dispatch(){
    thread_dispatch();

}

int Thread::sleep(time_t time){
    return time_sleep(time);
}

Thread::Thread(): Thread(runWrapper,this){
    //thread_create(&myHandle,runWrapper ,this);
}

//TODO
Thread::~Thread(){
    thread_dispatch();
    delete myHandle;
}

Semaphore::Semaphore (unsigned init){
    sem_open(&myHandle,init);
}

int Semaphore::wait(){
    return sem_wait(myHandle);
}

int Semaphore::signal(){
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t time){
    return sem_timedwait(myHandle,time);
}

int Semaphore::tryWait(){
    return sem_trywait(myHandle);
}

Semaphore::~Semaphore(){}

PeriodicThread::PeriodicThread(time_t period): Thread(){
    this->period = period;
}

void PeriodicThread::terminate(){
    isTerminated = true;
}

char Console::getc (){
    return _Console::getC();
}
void Console::putc (char c){
    _Console::putC(c);
}


