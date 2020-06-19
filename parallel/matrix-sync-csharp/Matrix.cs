using System;
using System.Numerics;

namespace matrixcsharp
{
    public class Matrix
    {
        private float[][] data;

        /// <summary>
        /// Number of rows
        /// </summary>
        public int M { get { return data.Length; } }

        /// <summary>
        /// Number of columns
        /// </summary>
        public int N { get { return data[0].Length; } }

        public float this[int i, int j]
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
            data = new float[sizeM][];
            for (int i = 0; i < M; ++i)
                data[i] = new float[sizeN];
        }

        public void FillValue(float val)
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
                float x = (float)rand.NextDouble();
                for (int j = 0; j < N; ++j)
                    data[i][j] = (float)rand.NextDouble() + x;
            }
        }

        private int rowcmp(float[] row1, float[] row2)
        {
            float sum1 = 0f;
            float sum2 = 0f;
            for (int j = 0; j < N; ++j)
            {
                sum1 += row1[j];
                sum2 += row2[j];
            }
            if (Math.Abs(sum2 - sum1) < float.Epsilon)
                return 0;
            if (sum2 < sum1)
                return -1;
            return 1;
        }

        public void Sort()
        {
            Array.Sort<float[]>(data, new Comparison<float[]>((row1, row2) => rowcmp(row2, row1)));
        }

        public void Add(Matrix other)
        {
            if (N != other.N)
                throw new ArgumentException(String.Format("A.N = {0} != B.N = {1}", N, other.N));
            if (M != other.M)
                throw new ArgumentException(String.Format("A.M = {0} != B.M = {1}", M, other.M));

            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j)
                    this[i, j] += other[i, j];
        }

        // Performs p*A*B+q*C operation using SIMD Vector<float>
        // Assumed to be used in parallel
        public static void AMultBPlusC(Matrix Result, Matrix A, Matrix B, Matrix C,
                                       float p = 1f, float q = 1f, int stride = 1, int offset = 0)
        {
            if (A.N != B.M)
                throw new ArgumentException(String.Format("A.N = {0} != B.M = {1}", A.N, B.M));
            if (A.M != C.N)
                throw new ArgumentException(String.Format("A.M = {0} != C.N = {1}", A.M, C.N));
            if (B.N != C.M)
                throw new ArgumentException(String.Format("B.N = {0} != C.M = {1}", B.N, C.M));
            if (A.M != Result.N)
                throw new ArgumentException(String.Format("A.M = {0} != Result.N = {1}", A.M, Result.N));
            if (B.N != Result.M)
                throw new ArgumentException(String.Format("B.N = {0} != Result.M = {1}", B.N, Result.M));

            const int simdVectorSize = 4;
            float[] abuffer = new float[simdVectorSize];
            float[] bbuffer = new float[simdVectorSize];

            for (int i = offset; i < Result.M; i += stride)
            {
                for (int j = 0; j < Result.N; ++j)
                {
                    float sum = 0f;
                    float kahan = 0f;
                    
                    for (int k = 0; k < Result.N; k += simdVectorSize)
                    {
                        for (int sk = 0; sk < simdVectorSize; ++sk)
                        {
                            if (k + sk < A.N)
                            {
                                abuffer[sk] = A[i, k + sk];
                                bbuffer[sk] = B[k + sk, j];
                            }
                            else
                            {
                                abuffer[sk] = 0f;
                                bbuffer[sk] = 0f;
                            }
                        }

                        Vector4 va = new Vector4(abuffer[0], abuffer[1], abuffer[2], abuffer[3]);
                        Vector4 vb = new Vector4(bbuffer[0], bbuffer[1], bbuffer[2], bbuffer[3]);

                        float dot = Vector4.Dot(va, vb); // A[i, k] * B[k, j]

                        // Kahan sum
                        float y = dot - kahan;
                        float t = sum + y;
                        kahan = (t - sum) - y;
                        sum = t;
                    }

                    //for (int k = 0; k < Result.N; ++k)
                    //    sum += A[i, k] * B[k, j];

                    Result[i, j] = p*sum + q*C[i, j];
                }
            }
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
                    float sum = 0f;
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
