//
// Created by os on 4/16/24.
//

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H


#include "../lib/hw.h"

class MemoryAllocator {
    struct DataNode{
        size_t size;
        DataNode* next = nullptr;
        DataNode* prev = nullptr;
        DataNode(size_t sz, DataNode* nx, DataNode* pv):size(sz), next(nx), prev(pv){}
    };

    static DataNode* headAllocated;

    static DataNode* headFree;

    static void tryToJoin(DataNode* p);
public:

   static const void* memAlloc(size_t size);

    static int free(const void * p);

    static void initFree(){

        size_t initFreeSize = (char*)HEAP_START_ADDR - (char*)HEAP_END_ADDR - sizeof(DataNode);
        //MemoryAllocator::headFree

        headFree= (DataNode*) ((char*)HEAP_START_ADDR);
        headFree ->size = initFreeSize;
        initFreeSize = headFree->size;
        headFree->next = nullptr;
        headFree->prev = nullptr;
    }

    //za testiranje

    static int getNumOfFreeBlocks();
};



#endif //MEMORYALLOCATOR_H
