//
// Created by os on 5/12/24.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../h/TCB.hpp"
#include "../h/Queue.hpp"
#include "../h/List.hpp"

class Scheduler {

    static Queue<TCB*> *threads;
    //static List<TCB*> *threads;
    static TCB* mainThr;


public:
    static void initialize();
    static TCB* get();
    static void put(TCB*);
    static void clear();


};



#endif //SCHEDULER_H