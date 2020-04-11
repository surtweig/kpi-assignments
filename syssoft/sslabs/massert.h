#include <iostream>

#define massert(Expr, Msg) \
    _massert(#Expr, Expr, __FILE__, __LINE__, Msg)

void _massert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}