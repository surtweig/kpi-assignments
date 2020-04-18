#include <iostream>
#include <cassert>
#include <math.h>

uint32_t FermatFactor(uint32_t n)
{
    assert(n % 2 == 1);
    uint32_t a = (uint32_t)sqrt(n);
    uint32_t b2 = a * a - n;
    double sqrtb2 = sqrt(b2);
    while (abs(sqrtb2 - floor(sqrtb2)) > DBL_EPSILON)
    {
        ++a;
        uint32_t a2 = a * a;
        if (a2 > n)
        b2 = a * a - n;
        sqrtb2 = sqrt(b2);
    }
    return (uint32_t)(a - sqrtb2);
}

uint32_t GetNextFactor(uint32_t n)
{
    if (n <= 1)
        return n;
    if (n % 2 == 0)
        return 2;
    uint32_t ff = FermatFactor(n);
    if (ff == 1)
        return n;
    else
        return ff;
}

void PrintFactors(uint32_t n)
{
    uint32_t f = GetNextFactor(n);
    uint32_t check = f;
    std::cout << n << " = \n" << " = " << f;
    while (f > 1)
    {
        n /= f;
        f = GetNextFactor(n);
        check *= f;
        if (f != 1)
            std::cout << " * " << f;
    }
    std::cout << " = \n = " << check << "\n";
}

int main()
{
    //std::cout << "Hello World!\n" << FermatFactor(259);
    uint32_t n = 1;
    while (n != 0)
    {
        std::cout << "Factorize (0 for exit): ";
        std::cin >> n;
        PrintFactors(n);
    }
}
