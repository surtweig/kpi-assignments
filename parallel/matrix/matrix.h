#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <assert.h>
#include <iostream>
#include <random>

using namespace std;

class matrix
{

private:
    float** elements;
    size_t _m;
    size_t _n;

public:
    // constructor
    matrix(size_t m, size_t n)
    {
        elements = new float*[n];
        for (size_t j = 0; j < n; ++j)
            elements[j] = new float[m];
        _n = n;
        _m = m;
    }

    // copy constructor
    matrix(const matrix& source)
    {
        _m = source.m();
        _n = source.n();

        elements = new float*[_n];
        for (size_t j = 0; j < _n; ++j)
        {
            elements[j] = new float[_m];
            for (size_t i = 0; i < _m; ++i)
                elements[j][i] = source.get(i, j);
        }
    }

    // copy assignment operator
    matrix& operator=(const matrix& source)
    {
        if(this != &source) {
            for (size_t j = 0; j < _n; ++j)
                delete elements[j];
            delete elements;

            _m = source.m();
            _n = source.n();

            elements = new float*[_n];
            for (size_t j = 0; j < _n; ++j)
            {
                elements[j] = new float[_m];
                for (size_t i = 0; i < _m; ++i)
                    elements[j][i] = source.get(i, j);
            }
        }
        return *this;
    }

    // destructor
    ~matrix()
    {
        for (size_t j = 0; j < _n; ++j)
            delete elements[j];
        delete elements;
    }

    inline size_t m() const { return _m; }
    inline size_t n() const { return _n; }
    float get(size_t i, size_t j) const { return elements[j][i]; }
    void set(size_t i, size_t j, float val) { elements[j][i] = val; }

    int rowcmp(size_t i1, size_t i2) const
    {
        float sum1 = 0.0;
        float sum2 = 0.0;
        for (size_t j = 0; j < _n; ++j)
        {
            sum1 += elements[j][i1];
            sum2 += elements[j][i2];
        }
        if (fabs(sum2-sum1) < numeric_limits<float>::epsilon())
            return 0;
        if (sum2 < sum1)
            return -1;
        return 1;
    }

    void rowswap(size_t i1, size_t i2)
    {
        for (size_t j = 0; j < _n; ++j)
        {
            float t = elements[j][i1];
            elements[j][i1] = elements[j][i2];
            elements[j][i2] = t;
        }
    }

    void sort(size_t first = 0, size_t last = size_t(-1))
    {
        if (last == size_t(-1))
            last = _m-1;

        if (first < last)
        {
            size_t count = last-first+1;
            if (count == 2)
            {
                if (rowcmp(last, first) == 1)
                    rowswap(last, first);
            }
            else
            {
                size_t pivot = first + (last-first)/2;
                size_t i1 = first;
                size_t i2 = last;
                while (true)
                {
                    while (rowcmp(i1, pivot) == 1)
                        ++i1;
                    while (rowcmp(pivot,i2) == 1)
                        --i2;
                    if (i1 >= i2)
                        break;
                    rowswap(i1, i2);
                }

                sort(first, i2);
                sort(i2+1, last);
            }
        }
    }


    void bubblesort(size_t first = 0, size_t last = size_t(-1))
    {
        if (last == size_t(-1))
            last = _m-1;

        for (size_t j = 0; j < _n; ++j)
        {

        }
    }


    float max() const
    {
        float x = elements[0][0];
        for (size_t i = 0; i < _m; ++i)
            for (size_t j = 0; j < _n; ++j)
                if (elements[j][i] > x)
                    x = elements[j][i];
        return x;
    }

    float min() const
    {
        float x = elements[0][0];
        for (size_t i = 0; i < _m; ++i)
            for (size_t j = 0; j < _n; ++j)
                if (elements[j][i] < x)
                    x = elements[j][i];
        return x;
    }

    void print(ostream& cout, string lineStart = "", string lineEnd = "") const
    {
        for (size_t i = 0; i < _m; ++i)
        {
            cout << lineStart;
            for (size_t j = 0; j < _n; ++j)
            {
                cout << fixed << elements[j][i];
                if (j < _n-1)
                    cout << "  ";
            }
            cout << lineEnd << "\n";
        }
        cout << "\n";
    }

    static matrix* identity(size_t m, size_t n)
    {
        matrix* result = new matrix(m, n);
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
            {
                if (i == j)
                    result->set(i, j, 1.0);
                else
                    result->set(i, j, 0.0);
            }
        return result;
    }

    static matrix* fill(size_t m, size_t n, float val)
    {
        matrix* result = new matrix(m, n);
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
                result->set(i, j, val);
        return result;
    }

    float realRand(const float & min, const float & max) {
        static std::mt19937 generator;
        std::uniform_real_distribution<float> distribution(min,max);
        return distribution(generator);
    }

    static matrix* random(size_t m, size_t n, float min, float max)
    {
        matrix* result = new matrix(m, n);
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
                result->set(i, j, result->realRand(min, max));
        return result;
    }

    static matrix* transponse(const matrix& source)
    {
        matrix* result = new matrix(source.n(), source.m());
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
                result->set(i, j, source.get(j, i));
        return result;
    }

    static matrix* multiply(const matrix& a, const matrix& b)
    {
        assert(a.n() == b.m());
        matrix* result = new matrix(a.m(), b.n());
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
            {
                float sum = 0.0;
                for (size_t k = 0; k < a.n(); ++k)
                    sum += a.get(i, k) * b.get(k, j);
                result->set(i, j, sum);
            }
        return result;
    }

    static matrix* add(const matrix& a, const matrix& b)
    {
        assert(a.n() == b.n() && a.m() == b.m());
        matrix* result = new matrix(a.m(), a.n());
        for (size_t i = 0; i < result->m(); ++i)
            for (size_t j = 0; j < result->n(); ++j)
            {
                result->set(i, j, a.get(i, j) + b.get(i, j));
            }
        return result;
    }

    static void addto(matrix& dest, const matrix& a)
    {
        assert(a.n() == dest.n() && a.m() == dest.m());
        for (size_t i = 0; i < dest.m(); ++i)
            for (size_t j = 0; j < dest.n(); ++j)
            {
                dest.set(i, j, a.get(i, j) + dest.get(i, j));
            }
    }

    static matrix* getcolumn(const matrix& source, size_t j)
    {
        matrix* result = new matrix(source.m(), 1);
        for (size_t i = 0; i < result->m(); ++i)
            result->set(i, 0, source.get(i, j));
        return result;
    }
};

#endif // MATRIX_H
