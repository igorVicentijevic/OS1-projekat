//
// Created by os on 4/16/24.
//

#include "../h/MemoryAllocator.hpp"

#include "../h/print.h"

MemoryAllocator::DataNode* MemoryAllocator::headFree = nullptr;
MemoryAllocator::DataNode* MemoryAllocator::headAllocated = nullptr;

const void* MemoryAllocator::memAlloc(size_t size){


    size_t numOfBlocks = size / MEM_BLOCK_SIZE + (size % MEM_BLOCK_SIZE > 0 ? 1:0);
    size_t sizeToAllocate = numOfBlocks * MEM_BLOCK_SIZE;

   DataNode* curr = headFree;


    while(curr!= nullptr && curr->size < sizeToAllocate )
        curr = curr ->next;

    if(curr == nullptr) return nullptr; //ne postoji free block zadate velicine

    //izlancaj curr iz liste free blokova
    if(curr->next) curr->next->prev = curr->prev;
    if(curr->prev) curr->prev->next = curr->next;
    if(headFree==curr) headFree=curr->next;

    curr->next = curr->prev = nullptr;


    //stavi alociran blok u listu alociranih blokova
    //curr->next = headAllocated;
    //curr->prev = nullptr;
    //if(headAllocated) headAllocated->prev = curr;
    //headAllocated = curr;

    size_t newFreeSize = curr->size - sizeToAllocate-sizeof(DataNode);

    if(newFreeSize < sizeof(DataNode)) return curr+sizeof(DataNode); //newFreeSize je premali da bi se ulancao u listu



    DataNode* nextPos = headFree;
    DataNode* prev = nullptr;

    // novu lokaciju u memoriji dobijemo tako sto na trenutnu adress
    // dodamo offset velicine koju alociramo
    DataNode* newFreeLoc =  (DataNode*)((char*) curr + sizeToAllocate+sizeof(DataNode));

    curr->size = sizeToAllocate;

    while(nextPos!=nullptr && nextPos->size < newFreeSize)
        nextPos=(prev = nextPos)->next;

    if(nextPos == nullptr){
        // ukoliko se smesta na kraj niza imamo dva posebna slucaja

        newFreeLoc->size = newFreeSize;

        if(prev == nullptr){
            //prvi slucaj: niz je prvobitno prazan, pa novi free block postaje head
            headFree = newFreeLoc;
            newFreeLoc->prev = newFreeLoc->next = nullptr;
        }
        else
        {
            //drugi slucaj: free block se samo dodaje na kraj niza
            prev->next = newFreeLoc;
            newFreeLoc->prev = prev;
        }
        return (char*)curr + sizeof(DataNode);
    }

    // slucaj kada free block ima i next i prev

    newFreeLoc -> next = nextPos;
    nextPos->prev = newFreeLoc;
    prev ->next = newFreeLoc;
    newFreeLoc -> prev = prev;

    //printInt(curr->size);

    printString("Nadjen je blok\n");

    return  (char*)curr+sizeof(DataNode);
}

int MemoryAllocator::free(const void* p){


    //DataNode* curr = headAllocated;
    //while(curr!=nullptr){

        //if(curr+sizeof(DataNode) == p){
            //break;
        //}
        //curr = curr->next;
    //}

    //if(curr==nullptr) return;

    //if(curr->next) curr->next->prev = curr->prev;
    //if(curr->prev) curr->prev->next = curr->next;
    //if(!curr->prev) headAllocated = curr->next;

    //curr->next = curr->prev = nullptr;

    if(p==nullptr) return -1;
    if((char*)p < (char*)HEAP_START_ADDR || (char*)p > (char*)HEAP_END_ADDR ) return -2;

    DataNode* curr = (DataNode*) ((char*)p-sizeof(DataNode));

    DataNode* f,*prev=nullptr;
    for(f = headFree; f!=nullptr && f->size<curr->size; prev =f, f=f->next);

    if(f==nullptr){
            //dodaje se na kraj liste
        if(prev) prev->next = curr;  else headFree=curr;
        curr->prev = prev;
        return 0;
    }

    curr->prev = f->prev;
    curr->next = f;
    if(curr->prev) curr->prev->next = curr; else headFree=curr;
    f->prev = curr;

    MemoryAllocator::tryToJoin(curr);
    MemoryAllocator::tryToJoin(curr->prev);

    return 0;


}

void MemoryAllocator::tryToJoin(DataNode* p){
    if(p == nullptr) return;

    DataNode* t = MemoryAllocator::headFree;
    t = t +0;
    if((char*)p->next == (char*)p + sizeof(DataNode) + p->size){
        //jedan do drugog su
        //moguce spajanje

        DataNode* next = p->next;
        if(next->next) next->next->prev = p;
        p->next = next->next;
        p->size+=next->size+sizeof(DataNode);
    }
}
int MemoryAllocator::getNumOfFreeBlocks(){
    int n = 0;

    for(DataNode* curr = headFree; curr!=nullptr; curr= curr->next, n++);
    return n;
}

