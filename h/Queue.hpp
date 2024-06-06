//
// Created by os on 5/12/24.
//

#ifndef QUEUE_H
#define QUEUE_H

#include "../h/TCB.hpp"
//#include "../h/print.h"

#define Q_SIZE 512

template <typename T>
class Queue {

    T *q; //q[Q_SIZE];
    int head=0;
    int tail=0;

    int size =0;


 public:

    Queue(int size = Q_SIZE) : size(size){
         q = new T[size];
     }


     void put(T toInsert){
           //printInt(head); printString("/"); printInt(tail);printString("\n");
         if((head+1)%size==tail) return; // red je pun

         q[head]=toInsert;
         head=(head+1)%size;
     }


     T* get(){
           //printInt(head); printString(":"); printInt(tail);printString("\n");
         if(head==tail) return nullptr; //red je prazan

         T* ret = &q[tail];
         tail = (tail+1)%size;

         return ret;
     }

     bool isEmpty() const{return head==tail; }

   /* ~Queue(){
       delete[] q;
     }*/




};



#endif //QUEUE_H
