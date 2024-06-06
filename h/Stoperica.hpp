//
// Created by os on 5/28/24.
//

#ifndef STOPERICA_H
#define STOPERICA_H


#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"
class Stoperica: public PeriodicThread{

    time_t period=0;
    time_t currentTime = 0;
    bool isPaused= false;
public:
    Stoperica(time_t period): PeriodicThread(period), period(period){}
    void pause(){isPaused = true;}
    void resume(){isPaused = false;}
    void reset(){currentTime = 0;}

    void periodicActivation () override
    {
        if(isPaused) return;
        currentTime +=period;
        printInt(currentTime);
        printString("\n");

    }

};



#endif //STOPERICA_H
