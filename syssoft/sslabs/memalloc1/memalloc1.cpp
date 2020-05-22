#include <iostream>
#include "Allocator.h"

int main()
{
    std::cout << "Hello World!\n";
    Allocator* allocator = new Allocator(1023);

    /*
    void* b1 = allocator->mem_alloc(256);
    void* b2 = allocator->mem_alloc(256);
    void* b3 = allocator->mem_alloc(256);
    allocator->mem_dump(std::cout);
    allocator->mem_free(b2);
    allocator->mem_dump(std::cout);
    allocator->mem_free(b1);
    allocator->mem_dump(std::cout);
    allocator->mem_free(b3);
    */

    /*
    void* b1 = allocator->mem_alloc(256);
    void* b2 = allocator->mem_alloc(512);
    //b1 = allocator->mem_realloc(b1, 512);
    allocator->mem_free(b1);
    allocator->mem_dump(std::cout);
    b2 = allocator->mem_realloc(b2, 864);

    allocator->mem_dump(std::cout);
    */

    std::cout << "Gamk? (1-yes, 2-no)\n> ";
    int x;
    std::cin >> x;

    delete allocator;
}