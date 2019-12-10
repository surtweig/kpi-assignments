//#define FORCE_SINGLE_THREAD
using System.Threading;
using System;

namespace matrixcsharp
{
    class MainClass
    {
        public const int msize = 5;
        public static object printLock = new object();

        public static void Main(string[] args)
        {
            /*
            Console.WriteLine("Hello World!");
            Matrix m = new Matrix(5, 7);
            m.FillRandom();
            m.Sort();
            m.Print();
            */
#if FORCE_SINGLE_THREAD
            F1();
            F2();
            F3();
#else
            Thread tf1 = new Thread(F1);
            tf1.Start();
            Thread tf2 = new Thread(F2);
            tf2.Start();
            Thread tf3 = new Thread(F3);
            tf3.Start();

            tf1.Join();
            tf2.Join();
            tf3.Join();
#endif
        }

        // 1.5 C = SORT(A) * (MA*ME) + SORT(B)
        public static void F1()
        {
            Matrix A = new Matrix(msize, 1);
            Matrix B = new Matrix(msize, 1);
            Matrix MA = new Matrix(msize, msize);
            Matrix ME = new Matrix(msize, msize);
            A.FillRandom();
            B.FillRandom();
            MA.FillRandom();
            ME.FillRandom();
            A.Sort();
            B.Sort();
            Matrix AE = MA.Multiply(ME);
            Matrix C = AE.Multiply(A);
            C.Add(B);
            lock (printLock)
            {
                Console.WriteLine("F1():");
                C.Print();
                Console.WriteLine();
            }
        }

        // 2.5 MG = SORT(MF) * MK + ML
        public static void F2()
        {
            Matrix MF = new Matrix(msize, msize);
            Matrix MK = new Matrix(msize, msize);
            Matrix ML = new Matrix(msize, msize);
            MF.FillRandom();
            MK.FillRandom();
            ML.FillRandom();

            MF.Sort();
            Matrix MG = MF.Multiply(MK);
            MG.Add(ML);
            lock (printLock)
            {
                Console.WriteLine("F2():");
                MG.Print();
                Console.WriteLine();
            }
        }

        // 3.5 O = (SORT(MP*MR)*S)
        public static void F3()
        {
            Matrix MP = new Matrix(msize, msize);
            Matrix MR = new Matrix(msize, msize);
            Matrix S = new Matrix(msize, 1);
            MP.FillRandom();
            MR.FillRandom();
            S.FillRandom();
            Matrix MPR = MP.Multiply(MR);
            MPR.Sort();
            Matrix O = MPR.Multiply(S);
            lock (printLock)
            {
                Console.WriteLine("F3():");
                O.Print();
                Console.WriteLine();
            }
        }
    }
}
