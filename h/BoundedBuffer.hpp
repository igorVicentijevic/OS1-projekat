//
// Created by os on 5/25/24.
//

#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include "../h/Queue.hpp"
#include "../h/syscall_c.hpp"
#include "../h/print.h"

#include "../h/_Semaphore.hpp"

template <typename T>
class BoundedBuffer{

    Queue<T> q;
   //sem_t spaceAvailable;
    //sem_t mutex;
    //sem_t itemAvailable;

    _Semaphore* spaceAvailable;
    _Semaphore* mutex;
    _Semaphore* itemAvailable;
    int size;


public:
    BoundedBuffer(int size):size(size),q(size+1){
       // printString("Pozvan konstruktor boundedBuffer\n");
      // sem_open(&spaceAvailable,size);
       //sem_open(&mutex,1);

       //sem_open(&itemAvailable,0);

       spaceAvailable = _Semaphore::sem_create(size);
       itemAvailable = _Semaphore::sem_create(0); //PREBACI NA 0
       mutex = _Semaphore::sem_create(1);


    }



    void put(T t){

        //sem_wait(spaceAvailable);
        //
        spaceAvailable->wait();
        //sem_wait(mutex);
       mutex->wait();
        q.put(t);
        //sem_signal(mutex);
        itemAvailable->signal();
       mutex->signal();

       //sem_signal(itemAvailable);


    }

    bool isEmpty(){
        return q.isEmpty();
    }

    T get(){
      //  sem_wait(itemAvailable);
        //sem_wait(mutex);
        itemAvailable->wait();
        mutex->wait();
        T *t = q.get();
       // sem_signal(spaceAvailable);
       spaceAvailable->signal();
       // sem_signal(mutex);
        mutex->signal();
        return *t;
    }


};
#endif //BOUNDEDBUFFER_H
