#include "../matrix/matrix.h"

#define MSIZE 5

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

#pragma omp critical
    {
        cout << "F1():\n";
        C->print(cout);
    }

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

#pragma omp critical
    {
        cout << "F2():\n";
        MFK->print(cout);
    }

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

#pragma omp critical
    {
        cout << "F3():\n";
        result->print(cout);
    }

    delete MP;
    delete MR;
    delete S;
    delete MRP;
    delete result;
}

int main(int argc, char *argv[])
{
#pragma omp parallel sections
    {
#pragma omp section
        F1();
#pragma omp section
        F2();
#pragma omp section
        F3();
    }
}
