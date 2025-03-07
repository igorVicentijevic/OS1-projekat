//
// Created by os on 5/12/24.
//

#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"
#include "../h/List.hpp"


class TCB {
    static uint64 constexpr STACK_SIZE=DEFAULT_STACK_SIZE;//4096;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;//2;
    using Body = void (*)(void*);

    static uint64 timeSliceCounter;

	static List<TCB*>* sleepingThreads;



    uint64 pid;

    static uint64 ID;

	time_t timeToSleep;



    struct Context{
        uint64 ra;
        uint64 sp;

    };

    Body body;

    void* args;

    uint64 *stack=0;
    Context context;

    uint64 timeSlice;

    bool finished = false;

	static void putTCBToSleep(TCB* thrToSleep);


    static void dispatch();

    static void contextSwitch(Context* oldCon, Context* newCon);

    TCB(Body body, void* args,void* stack_space, uint64 timeSlice): body(body),args(args),
    stack((uint64*) stack_space),timeSlice(timeSlice){

        pid=++ID;
        setFinished(false);
    	//stack = new uint64[DEFAULT_STACK_SIZE];

    	context = {(uint64)TCB::threadWrapper, (uint64) (stack+DEFAULT_STACK_SIZE)};

    }

    bool hasStarted = false;
    friend class Riscv;
    friend class _Semaphore;
    static void threadWrapper();

	bool putBackInScheduler = true;

static void updateSleepingThreadsTime();



public:


  static uint64 totalTimeSliceCounter;
    static TCB* createThread(Body body,void* args,void* stack_space, uint64 timeSlice = DEFAULT_TIME_SLICE);

    static TCB* running;

    void start();
    void join();

    static void yield();

    static void removeTCBfromSleepingThreads(TCB *tcb);

    uint64 getPID() const {return pid;}
    uint64 getTimeSlice() const {return timeSlice;}

    bool isFinished() const {return finished;}
    void setFinished(bool f) {finished=f;}

	time_t getTimeToSleep() const {return timeToSleep;}
	void setTimeToSleep(time_t time) { timeToSleep = time;}

	bool getPutBackInScheduler() const {return putBackInScheduler;}


  ~TCB(){delete[] stack;}
};



#endif //TCB_H
