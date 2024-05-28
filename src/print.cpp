//
// Created by os on 4/18/24.
//

#include "../h/print.h"
#include "../h/Riscv.hpp"

#include "../h/syscall_c.hpp"

#include "../lib/console.h"
#include "../lib/hw.h"



void printString(const char* str){


   // uint64 sstatus = Riscv::r_sstatus();
    //Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    while(*str != '\0')
    {
        //__putc(*str);
        put_c(*str);
        str++;
    }
   //Riscv::ms_sstatus(sstatus&Riscv::SSTATUS_SIE);
}

void printInt(unsigned long long x){

    //uint64 sstatus = Riscv::r_sstatus();
    //Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
   unsigned long long n = 10;
    while(n<=x)
        n*=10;
    n/=10;

    while(n>0){
        int num = x/n;
        x%=n;
        n/=10;
       //__putc('0'+num);
       put_c('0'+num);
    }
    //Riscv::ms_sstatus(sstatus&Riscv::SSTATUS_SIE);


}


