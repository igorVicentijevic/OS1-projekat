//
// Created by os on 5/8/24.
//

#include "../h/Riscv.hpp"
#include "../h/TCB.hpp"
//#include "../h/print.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_Semaphore.hpp"
#include "../h/Console.hpp"

#include "../lib/mem.h"
#include "../lib/console.h"



void Riscv::popSppSpie(){

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap(){
        uint64 scause = r_scause();
        //printInt(scause);

        if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
            //interrupt: no, cause code: enveiroment  call from s-mode
            uint64 volatile sepc;
            sepc = r_sepc();
            sepc+= 4;
            uint64 volatile sstatus = r_sstatus();

            char c;

            uint64 codeOperation = Riscv::r_a0();
            switch(codeOperation){
                case 0x01:
                    //memAlloc(size)

                     size_t size;
                     void* ptr;
                    __asm__ volatile ("mv %0, a1" : "=r"(size));

                    //moj alokator
                    //ptr = (void*) MemoryAllocator::memAlloc(size);
                    ptr = __mem_alloc(size);

                    __asm__ volatile("mv t0, %0" : : "r"(ptr));
                    __asm__ volatile ("sw t0, 80(x8)");
                    break;

                case 0x02:
                    //memFree(ptr)

                    void* p;
                    __asm__ volatile ("mv %0, a1" : "=r"(p));

                    // moj alokator
                    //int ret = MemoryAllocator::free(p);

                    int ret;
                    ret = __mem_free(p);

                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");
                    break;

                case 0x11:
                    //thread_create __with start
                     TCB** handle;
                     __asm__ volatile ("mv %0, a1" : "=r"(handle));
                    void(*body)(void*);
                     __asm__ volatile ("mv %0, a2" : "=r"(body));
                    void* args;
                    __asm__ volatile ("mv %0, a7" : "=r"(args));


                    *handle = TCB::createThread(body, args);
					(*handle)->start();
                    ret = 0;
                    if(*handle==nullptr) ret = -1;
                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");

                    break;
                case 0x12:
                    //thread_exit
                    TCB::running->setFinished(true);
                    TCB::dispatch();
                    break;

                case 0x13:
                    //thread_dispatch
                    TCB::timeSliceCounter = 0;
                    TCB::dispatch();
                    break;

                case 0x21:
                      _Semaphore** handle_sem;
                     __asm__ volatile ("mv %0, a1" : "=r"(handle_sem));
                    unsigned val;
                     __asm__ volatile ("mv %0, a2" : "=r"(val));

                    *handle_sem = _Semaphore::sem_create(val);

                     ret = 0;
                    if(*handle_sem==nullptr) ret = -1;
                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");

                     break;
                 case 0x22:
                        // sem_close
                      _Semaphore* handle_s;
                     __asm__ volatile ("mv %0, a1" : "=r"(handle_s));

                        ret = 0;

                      if(handle_s==nullptr) ret = -1;
                      else handle_s->close();



                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");

                     break;
                 case 0x23:
                         //sem_wait
                     __asm__ volatile ("mv %0, a1" : "=r"(handle_s));

                        ret = 0;

                      if(handle_s==nullptr) ret = -1;
                      else handle_s->wait();



                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");

                     break;

                  case 0x24:
                         //sem_signal
                     __asm__ volatile ("mv %0, a1" : "=r"(handle_s));

                        ret = 0;

                      if(handle_s==nullptr) ret = -1;
                      else handle_s->signal();



                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");

                     break;

                 case 0x25:
                    //sem_timewait
                    __asm__ volatile ("mv %0, a1" : "=r"(handle_s));
                    time_t timeout;
                    __asm__ volatile ("mv %0, a2" : "=r"(timeout));

                     ret = handle_s == nullptr ? -1: handle_s->timedWait(timeout);

                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");
                    break;
				 case 0x26:

					//sem_trywait
  					__asm__ volatile ("mv %0, a1" : "=r"(handle_s));

                        ret = 0;

                      if(handle_s==nullptr) ret = -1;
                      else ret = handle_s->tryWait();



                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");
					break;
 				case 0x31:

					//time_sleep
					time_t sleepTime;
  					__asm__ volatile ("mv %0, a1" : "=r"(sleepTime));

                        ret = 0;
						TCB::running->timeToSleep = sleepTime;
						TCB::putTCBToSleep(TCB::running);

                    __asm__ volatile("mv t0, %0" : : "r"((uint64)ret));
                    __asm__ volatile ("sw t0, 80(x8)");
					break;

                case 0x41:

				    c = Console::getC();

                    __asm__ volatile("mv t0, %0" : : "r"((uint64)c));
                    __asm__ volatile ("sw t0, 80(x8)");
					break;

                case 0x42:

  					__asm__ volatile ("mv %0, a1" : "=r"(c));

                    Console::putC(c);
				break;

            }

            __asm__ volatile ("csrw sepc, %0" : :"r"(sepc));
            __asm__ volatile ("csrw sstatus, %0" : :"r"(sstatus));

                        //pozvan iz korisnickog ili sistemskog rezima
        }
        else if (scause == 0x8000000000000001UL){
            //interrupt: yes, cause code: supervisor software interrupt (timer) 10HZ

			//printString("Tajmer!\n");
            Console::handlerOutput();

            if(true){
            TCB::timeSliceCounter++;
            TCB::totalTimeSliceCounter++;

			TCB::updateSleepingThreadsTime();


            if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){

                uint64 sepc = r_sepc(); //kontekst ulazne niti

                if(sepc == 10){
                    //printString("uso");
                    //int x;
                    //__asm__ volatile ("mv x0,%0"::"r"(x));
                }

                uint64 sstatus = r_sstatus();
                TCB::timeSliceCounter=0;

                TCB::dispatch(); //dolazi do promene niti

                w_sepc(sepc); // kontekst izlazne niti
                w_sstatus(sstatus);
            }
            }

             mc_sip(SIP_SSIP); //obradjen softverski prekid
        }
        else if(scause == 0x8000000000000009UL){
            //interrupt: yes, cause code: supervisor external interrupt (console)
            //console_handler();
            //Console::handler();
            Console::handlerInput();
        }
        else{
            //unexpected scause
            //print(scause)
            //print(sepc)
            //print(stval)
        }
}