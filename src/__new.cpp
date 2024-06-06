//
// Created by os on 5/12/24.
//

#include "../h/syscall_c.hpp"
#include "../lib/mem.h"
void* operator new(size_t sz){
   return mem_alloc(sz);

}

void* operator new[](size_t sz){
    return mem_alloc(sz);
}

void operator delete(void* ptr) noexcept{
    mem_free(ptr);

}

void operator delete[](void* ptr) noexcept{
    __mem_free(ptr);
}