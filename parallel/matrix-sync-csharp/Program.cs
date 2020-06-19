using System;
using System.Threading;
using matrixcsharp;


namespace matrix_sync_csharp
{

    class AMultBPlusCThreadGroup
    {
        private Thread[] threads;
        private Matrix result;
        private Matrix ma;
        private Matrix mb;
        private Matrix mc;
        private float p;
        private float q;
        public string DisplayName;

        private static object printLock = new object();

        public AMultBPlusCThreadGroup(int threadCount)
        {
            threads = new Thread[threadCount];
        }

        //q*A*B + p*C
        public void Start(string DisplayName, Matrix Result, Matrix A, Matrix B, Matrix C, float p, float q)
        {
            result = Result;
            ma = A;
            mb = B;
            mc = C;
            this.p = p;
            this.q = q;
            this.DisplayName = DisplayName;

            for (int i = 0; i < threads.Length; ++i)
            {
                int ii = i; // wtf lambda closure
                threads[i] = new Thread(() => threadProc(ii));
                threads[i].Start();
            }
        }

        private void threadProc(int threadIndex)
        {
            lock (printLock)
                Console.WriteLine(string.Format("{0} thread{1} started.", DisplayName, threadIndex));

            Matrix.AMultBPlusC(result, ma, mb, mc, p, q, threads.Length, threadIndex);

            lock (printLock)
                Console.WriteLine(string.Format("{0} thread{1} finished.", DisplayName, threadIndex));
        }

        public void Join()
        {
            foreach (Thread t in threads)
            {
                if (t != null)
                    t.Join();
            }
        }

        public void Abort()
        {
            foreach (Thread t in threads)
            {
                if (t != null)
                    t.Abort();
            }
        }
    }

    class Program
    {

        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            Matrix MA = new Matrix(10, 10);
            Matrix MB = new Matrix(10, 10);
            Matrix MK = new Matrix(10, 10);
            Matrix MC = new Matrix(10, 10);
            Matrix MX = new Matrix(10, 10);
            Matrix MT = new Matrix(10, 10);
            Matrix MM = new Matrix(10, 10);

            MB.FillRandom();
            MK.FillRandom();
            MC.FillRandom();
            MX.FillRandom();
            MT.FillRandom();
            MM.FillRandom();

            Matrix _Mzero = new Matrix(10, 10);
            Matrix _Mxtm = new Matrix(10, 10);
            Matrix _Mbk = new Matrix(10, 10);

            /*
               МА = МВ*MK - МС * (МХ*MT + MM);
               Mxtm = AMultBPlusC(MX, MT, MM)
               Mbk = AMultBPlusC(MB, MK, _Mzero)
               MA = AMultBPlusC(-MC, Mxtm, Mbk)
            */

            AMultBPlusCThreadGroup abcthreads1 = new AMultBPlusCThreadGroup(3);
            AMultBPlusCThreadGroup abcthreads2 = new AMultBPlusCThreadGroup(3);

            abcthreads1.Start("MX*MT + MM", _Mxtm, MX, MT, MM, 1f, 1f);
            abcthreads2.Start("MB*MK", _Mbk, MB, MK, _Mzero, 1f, 1f);
            abcthreads1.Join();
            abcthreads2.Join();

            abcthreads1.Start("Mbk - MC*Mxtm", MA, MC, _Mxtm, _Mbk, -1f, 1f);
            abcthreads1.Join();

            MA.Print();
        }
    }
}
