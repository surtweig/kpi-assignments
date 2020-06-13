//#include <QCoreApplication>

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "../matrix/matrix.h"
#include <cmath>
#include <chrono>
#include <condition_variable>
#include <atomic>

using namespace std;

#define MSIZE 5

mutex initMut;
mutex multMut;
mutex printMut;
condition_variable cv;
atomic<int> multThreads;
atomic<int> initThreads;

void t_init(string name, matrix* &mat, size_t m, size_t n)//char* &name,
{
    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_init(" << name << ") starts.\n";
    }

    // init matrix
    mat = matrix::random(m, n, 0.0, 1.0);

    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_init(" << name << ") finishes:\n";
        cout << "\n\t" << name << " = \n";
        mat->print(cout, "\t[ ", " ]");
    }

    // operation finished, notify mult threads
    unique_lock<mutex> initlock(initMut);
    --initThreads;
    initlock.unlock();
    cv.notify_one();
}

void t_mult(string name, matrix* &m1, matrix* &m2, matrix* &result)
{
    // waiting init threads
    unique_lock<mutex> initlock(initMut);
    cv.wait(initlock, [] { return initThreads <= 0; });

    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_mult(" << name << ") starts.\n";
    }

    // multiply
    result = matrix::multiply(*m1, *m2);

    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_mult(" << name << ") finishes.\n";
    }

    // operation finished, notify sum thread
    unique_lock<mutex> multlock(multMut);
    --multThreads;
    multlock.unlock();
    cv.notify_one();
}

void t_sum(matrix* &mdest, matrix* &m2, matrix* &m3)
{
    // waiting mult threads
    unique_lock<mutex> lock(multMut);
    cv.wait(lock, [] { return multThreads <= 0; });

    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_sum starts.\n";
    }

    // adding
    matrix::addto(*mdest, *m2);
    matrix::addto(*mdest, *m3);

    {
        lock_guard<mutex> printLock(printMut);
        cout << "t_sum finishes:\nResult=\n";
        mdest->print(cout, "\t[ ", " ]");
    }
}

// A = B*MC + D*MZ + E*MM
int main(int argc, char *argv[])
{
    multThreads = 3;
    initThreads = 6;

    cout << "Hello, World!\n";

    matrix* BMC = nullptr;
    matrix* DMZ = nullptr;
    matrix* EMM = nullptr;
    matrix* B = nullptr;
    matrix* MC = nullptr;
    matrix* D = nullptr;
    matrix* MZ = nullptr;
    matrix* E = nullptr;
    matrix* MM = nullptr;

    thread t_initB (t_init, string("B") , ref(B), 1, MSIZE);      //  "B"
    thread t_initD (t_init, string("D") , ref(D), 1, MSIZE);      //  "D"x
    thread t_initE (t_init, string("E") , ref(E), 1, MSIZE);      //  "E"
    thread t_initMC(t_init, string("MC"), ref(MC), MSIZE, MSIZE); //  "MC"
    thread t_initMZ(t_init, string("MZ"), ref(MZ), MSIZE, MSIZE); //  "MZ"
    thread t_initMM(t_init, string("MM"), ref(MM), MSIZE, MSIZE); //  "MM"

    thread t_multBMC(t_mult, string("B*MC"), ref(B), ref(MC), ref(BMC));
    thread t_multDMZ(t_mult, string("D*MZ"), ref(D), ref(MZ), ref(DMZ));
    thread t_multEMM(t_mult, string("E*MM"), ref(E), ref(MM), ref(EMM));

    thread t_sumA(t_sum, ref(BMC), ref(DMZ), ref(EMM));

    t_sumA.join();


    delete BMC;
    delete DMZ;
    delete EMM;
    delete B;
    delete MC;
    delete D;
    delete MZ;
    delete E;
    delete MM;

    return 0;
}
