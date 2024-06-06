//
// Created by os on 5/20/24.
//

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "../h/Queue.hpp"
#include "../h/TCB.hpp"
#include "../h/List.hpp"

class _Semaphore {
    int val;

    int ret = 0;

    //Queue<TCB*> blocked;
    List<TCB*> blocked;
    //List<TCB*> sleeping;
    _Semaphore(int val);

   int  waitWithoutDispatch();



public:

    static _Semaphore* sem_create(int val);

    int wait();
    int signal();

    int timedWait(time_t timeout);

	int tryWait();

    void close();



};



#endif //_SEMAPHORE_H
