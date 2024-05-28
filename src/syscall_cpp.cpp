//
// Created by os on 5/28/24.
//

#include "../h/syscall_cpp.hpp"
void Thread::runWrapper(void * arg){
    ((Thread*) arg)->run();
}

Thread::Thread(void (*body)(void*), void* arg){
   this-> body = body;
   this->arg = arg;
    thread_create(&myHandle, body, arg);
}

int Thread::start(){
    //TODO
}

void Thread::dispatch(){
    thread_dispatch();
}

int Thread::sleep(time_t time){
    return time_sleep(time);
}

Thread::Thread(){
    thread_create(&myHandle,runWrapper ,this);
}

Thread::~Thread(){}

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


