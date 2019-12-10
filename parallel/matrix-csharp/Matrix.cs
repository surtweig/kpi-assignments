using System;

namespace matrixcsharp
{
    public class Matrix
    {
        private double[][] data;

        /// <summary>
        /// Number of rows
        /// </summary>
        public int M { get { return data.Length; } }

        /// <summary>
        /// Number of columns
        /// </summary>
        public int N { get { return data[0].Length; } }

        public double this[int i, int j]
        {
            get
            {
                return data[i][j];
            }
            set
            {
                data[i][j] = value;
            }
        }

        public Matrix(int sizeM, int sizeN)
        {
            data = new double[sizeM][];
            for (int i = 0; i < M; ++i)
                data[i] = new double[sizeN];
        }

        public void FillValue(double val)
        {
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    data[i][j] = val;
        }

        public void FillRandom()
        {
            Random rand = new Random();
            for (int i = 0; i < M; ++i)
            {
                double x = rand.NextDouble();
                for (int j = 0; j < N; ++j)
                    data[i][j] = rand.NextDouble() + x;
            }
        }

        private int rowcmp(double[] row1, double[] row2)
        {
            double sum1 = 0.0;
            double sum2 = 0.0;
            for (int j = 0; j < N; ++j)
            {
                sum1 += row1[j];
                sum2 += row2[j];
            }
            if (Math.Abs(sum2 - sum1) < double.Epsilon)
                return 0;
            if (sum2 < sum1)
                return -1;
            return 1;
        }

        public void Sort()
        {
            Array.Sort<double[]>(data, new Comparison<double[]>((row1, row2) => rowcmp(row2, row1)));
        }

        public void Add(Matrix other)
        {
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    this[i, j] += other[i, j];
        }

        public Matrix Multiply(Matrix other)
        {
            if (N != other.M)
                throw new ArgumentException(String.Format("A.N = {0} != B.M = {1}", N, other.M));

            Matrix mult = new Matrix(M, other.N);
            for (int i = 0; i < mult.M; ++i)
            {
                for (int j = 0; j < mult.N; ++j)
                {
                    double sum = 0.0;
                    for (int k = 0; k < N; ++k)
                        sum += this[i, k] * other[k, j];
                    mult[i, j] = sum;
                }
            }
            return mult;
        }

        public void Print()
        {
            for (int i = 0; i < M; ++i)
            {
                for (int j = 0; j < N; ++j)
                    Console.Write(String.Format("{0:0.00}", data[i][j]) + "\t");
                Console.WriteLine();
            }
        }
    }
}
