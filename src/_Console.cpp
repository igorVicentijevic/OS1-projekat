//
// Created by os on 5/25/24.
//

#include "../h/_Console.hpp"
#include "../lib/hw.h"
#include "../test/buffer.hpp"

#define BB_SIZE 256//2048//256//1024
BoundedBuffer<char> *  _Console::putCbuffer=nullptr;
BoundedBuffer<char>  * _Console::getCbuffer=nullptr;
//Buffer* _Console::getCbuffer = nullptr;
//Buffer* _Console::putCbuffer = nullptr;

char _Console::temp = 'z';

void _Console::initialize(){
    putCbuffer = new BoundedBuffer<char>(BB_SIZE);
    getCbuffer = new BoundedBuffer<char>(BB_SIZE);
    //putCbuffer = new Buffer(BB_SIZE);
    //getCbuffer = new Buffer(BB_SIZE);
}

void _Console::handlerOutput(){
    //if(getCbuffer==nullptr) getCbuffer = new BoundedBuffer<>(512);

    while( (*((char*)CONSOLE_STATUS) & STATUS_CAN_PUT_C)) {
        if(putCbuffer->isEmpty()) return;
       // if(putCbuffer->getCnt() ==0) return;
        char c = putCbuffer->get();
        *((char*)CONSOLE_TX_DATA) = c;
    }
}

bool _Console::buffersAreEmpty(){
    return putCbuffer->isEmpty(); //&& getCbuffer -> isEmpty();<
   // return putCbuffer->getCnt() ==0;
}

void _Console::handlerInput(){
    if(plic_claim() != CONSOLE_IRQ) return;

    while( (*((char*)CONSOLE_STATUS) & STATUS_CAN_GET_C)){
		char c = *((char*) CONSOLE_RX_DATA);
      //  if(c=='\r') continue;
        getCbuffer->put(c);

    }
//plic_complete(irq);
    plic_complete(CONSOLE_IRQ);
}
void _Console::putC(char c){

    putCbuffer->put(c);
}
char _Console::getC(){

    return getCbuffer->get();

}