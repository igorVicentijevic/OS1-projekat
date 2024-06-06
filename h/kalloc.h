//
// Created by os on 5/31/24.
//

#ifndef KALLOC_H
#define KALLOC_H
#include "../lib/mem.h"

void* kmalloc(size_t sz)
{
    return __mem_alloc(sz);
 }

 void kfree(void * ptr){
     __mem_free(ptr);
 }
#endif //KALLOC_H
