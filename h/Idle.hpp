//
// Created by os on 5/25/24.
//

#ifndef IDLE_H
#define IDLE_H

#include "../h/TCB.hpp"

class Idle{
    static TCB* idle;
    static void idleBody(void* arg);

public:
    static TCB* getIdle();
};


#endif //IDLE_H
