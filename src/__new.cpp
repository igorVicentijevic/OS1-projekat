//
// Created by os on 5/12/24.
//

#include "../h/syscall_c.hpp"

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
    mem_free(ptr);
}