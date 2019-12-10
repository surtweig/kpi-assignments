//#define FORCE_SINGLE_THREAD

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "matrix.h"
#include <cmath>
#include <chrono>

#define MSIZE 5

using namespace std;

mutex printlock;

void test()
{
    matrix* m1 = matrix::random(3, 3, 0.0, 1.0);
    matrix* m2 = matrix::fill(3, 3, 1.0);
    m1->print(cout);
    cout << "\n";
    m2->print(cout);

    matrix* m3 = matrix::multiply(*m1, *m2);
    m3->print(cout);

    delete m1;
    delete m2;
    delete m3;
}

void test2()
{
    matrix* a = matrix::random(3, 7, 0.0, 1.0);
    matrix* b = matrix::random(7, 6, 0.0, 1.0);
    matrix* c = matrix::multiply(*a, *b);
    matrix* v = matrix::random(10, 1, 0.0, 1.0);
    c->print(cout);
    v->print(cout);
    v->sort();
    v->print(cout);

    delete a;
    delete b;
    delete c;
}


// 1.5 C = SORT(A) *(MA*ME) + SORT(B)
void F1()
{
    matrix* MA = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* ME = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* A = matrix::random(MSIZE, 1, 0.0, 1.0);//matrix::getcolumn(*MA, 0);
    matrix* B = matrix::random(MSIZE, 1, 0.0, 1.0);

    matrix* ae = matrix::multiply(*MA, *ME);
    A->sort();
    B->sort();
    matrix* aae = matrix::multiply(*ae, *A);

    matrix* C = matrix::add(*aae, *B);
    printlock.lock();
    cout << "F1():\n";
    C->print(cout);
    printlock.unlock();

    delete MA;
    delete ME;
    delete A;
    delete B;
    delete C;
    delete ae;
    delete aae;
}

// 2.5 MG = SORT(MF) * MK + ML
void F2()
{
    matrix* MF = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* MK = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* ML = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    MF->sort();
    matrix* MFK = matrix::multiply(*MF, *MK);
    matrix::addto(*MFK, *ML);
    printlock.lock();
    cout << "F2():\n";
    MFK->print(cout);
    printlock.unlock();
    delete MF;
    delete MK;
    delete ML;
    delete MFK;
}

// 3.5 O = (SORT(MP*MR)*S)
void F3()
{
    matrix* MP = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* MR = matrix::random(MSIZE, MSIZE, 0.0, 1.0);
    matrix* S = matrix::random(MSIZE, 1, 0.0, 1.0);

    matrix* MRP = matrix::multiply(*MP, *MR);
    MRP->sort();
    matrix* result = matrix::multiply(*MRP, *S);
    printlock.lock();
    cout << "F3():\n";
    result->print(cout);
    printlock.unlock();

    delete MP;
    delete MR;
    delete S;
    delete MRP;
    delete result;
}

int main(int argc, char *argv[])
{
#ifdef FORCE_SINGLE_THREAD
    auto begin = std::chrono::high_resolution_clock::now();
    F1();
    F2();
    F3();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us" << std::endl;
#else
    auto begin = std::chrono::high_resolution_clock::now();
    thread tf1(F1);
    thread tf2(F2);
    thread tf3(F3);
    tf1.join();
    tf2.join();
    tf3.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us" << std::endl;
#endif
    cout << "\n";
    return 0;
}
