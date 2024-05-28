//
// Created by os on 5/25/24.
//

#include "../h/Console.hpp"
#include "../lib/hw.h"
#include "../Test/buffer.hpp"

#define BB_SIZE 256//2048//256//1024
//BoundedBuffer<char> *  Console::putCbuffer=nullptr;
//BoundedBuffer<char>  * Console::getCbuffer=nullptr;
Buffer* Console::getCbuffer = nullptr;
Buffer* Console::putCbuffer = nullptr;

char Console::temp = 'z';

void Console::initialize(){
    //putCbuffer = new BoundedBuffer<char>(BB_SIZE);
    //getCbuffer = new BoundedBuffer<char>(BB_SIZE);
    putCbuffer = new Buffer(BB_SIZE);
    getCbuffer = new Buffer(BB_SIZE);
}

void Console::handlerOutput(){
    //if(getCbuffer==nullptr) getCbuffer = new BoundedBuffer<>(512);

    while( (*((char*)CONSOLE_STATUS) & STATUS_CAN_PUT_C)) {
       // if(putCbuffer->isEmpty()) return;
        if(putCbuffer->getCnt() ==0) return;
        char c = putCbuffer->get();
        *((char*)CONSOLE_TX_DATA) = c;
    }
}

bool Console::buffersAreEmpty(){
   // return putCbuffer->isEmpty(); //&& getCbuffer -> isEmpty();<
    return putCbuffer->getCnt() ==0;
}

void Console::handlerInput(){
    if(plic_claim() != CONSOLE_IRQ) return;

    while( (*((char*)CONSOLE_STATUS) & STATUS_CAN_GET_C)){
		char c = *((char*) CONSOLE_RX_DATA);
      //  if(c=='\r') continue;
        getCbuffer->put(c);

    }
//plic_complete(irq);
    plic_complete(CONSOLE_IRQ);
}
void Console::putC(char c){

    putCbuffer->put(c);
}
char Console::getC(){

    return getCbuffer->get();

}