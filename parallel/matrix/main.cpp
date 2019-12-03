#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "matrix.h"

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

void F1()
{
    matrix* MA = matrix::random(5, 5, 0.0, 1.0);
    matrix* ME = matrix::random(5, 5, 0.0, 1.0);
    matrix* A = matrix::random(5, 1, 0.0, 1.0);//matrix::getcolumn(*MA, 0);
    matrix* B = matrix::random(5, 1, 0.0, 1.0);

    //MA->print(cout);
    //ME->print(cout);
    //A->print(cout);
    //B->print(cout);

    matrix* ae = matrix::multiply(*MA, *ME);
    A->sort();
    //matrix* AT = matrix::transponse(*A);
    B->sort();
    //B->print(cout);
    matrix* aae = matrix::multiply(*ae, *A);
    //C->print(cout);
    //aae->print(cout);

    matrix* C = matrix::add(*aae, *B);
    printlock.lock();
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

void F2()
{
    matrix* MF = matrix::random(5, 5, 0.0, 1.0);
    matrix* MK = matrix::random(5, 5, 0.0, 1.0);
    matrix* ML = matrix::random(5, 5, 0.0, 1.0);
    MF->sort();
    matrix* MFK = matrix::multiply(*MF, *MK);
    matrix::addto(*MFK, *ML);
    printlock.lock();
    MFK->print(cout);
    printlock.unlock();
    delete MF;
    delete MK;
    delete ML;
    delete MFK;
}

void F3()
{
    matrix* MP = matrix::random(5, 5, 0.0, 1.0);
    matrix* MR = matrix::random(5, 5, 0.0, 1.0);
    matrix* S = matrix::random(5, 1, 0.0, 1.0);

    matrix* MRP = matrix::multiply(*MP, *MR);
    MRP->sort();
    matrix* result = matrix::multiply(*MRP, *S);
    printlock.lock();
    result->print(cout);
    printlock.unlock();

    delete MP;
    delete MR;
    delete S;
    delete MRP;
    delete result;
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

int main(int argc, char *argv[])
{

    //F1();
    //test2();
    thread tf1(F1);
    thread tf2(F2);
    thread tf3(F3);

    tf1.join();
    tf2.join();
    tf3.join();
    //tf1.join();
    cout << "\n";
    return 0;
}

