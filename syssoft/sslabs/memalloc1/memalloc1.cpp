#include <iostream>
#include "Allocator.h"

int main()
{
    std::cout << "Hello World!\n";
    Allocator* allocator = new Allocator(1023);

    allocator->mem_dump(std::cout);

    delete allocator;
}