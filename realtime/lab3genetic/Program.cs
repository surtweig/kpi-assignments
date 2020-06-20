using System;
using System.Collections.Generic;
using System.Text;

namespace lab3genetic
{
    public class GenotypeDiophantine
    {
        private int[] genes;
        public int TestedDelta { get; private set; }
        public static Random rand = new Random();

        public GenotypeDiophantine(int[] genes)
        {
            this.genes = new int[genes.Length];
            Array.Copy(genes, 0, this.genes, 0, this.genes.Length);
            //Console.WriteLine(string.Format("initial GenotypeDiophantine {0}", genes));
        }


        public GenotypeDiophantine(GenotypeDiophantine parentA, GenotypeDiophantine parentB)
        {
            if (parentA.genes.Length != parentB.genes.Length)
                throw new ArgumentException(string.Format("parentA has {0} genes, parentB has {1}", parentA.genes.Length, parentB.genes.Length));

            genes = new int[parentA.genes.Length];

            // shuffle parents
            GenotypeDiophantine p1 = parentA;
            GenotypeDiophantine p2 = parentB;
            if (rand.Next() % 2 == 1)
            {
                p1 = parentA;
                p2 = parentB;
            }

            // crossover line
            int crossover = rand.Next(1, genes.Length-1);
            // 1 2 3 4
            // 5 6 7 8
            // cross = 1
            // 1 6 7 8
            Array.Copy(p1.genes, 0, genes, 0, crossover);
            Array.Copy(p2.genes, crossover, genes, crossover, genes.Length - crossover);

            //Console.WriteLine(string.Format("spawned GenotypeDiophantine {0}", genes));
        }

        public int TestDiophantineEquation(int[] factors)
        {
            if (factors.Length - 1 != genes.Length)
                throw new ArgumentException(string.Format("{0} factors, {1} genes", factors.Length, genes.Length));
            int s = factors[0];
            for (int i = 0; i < genes.Length; ++i)
                s += factors[i + 1] * genes[i];

            TestedDelta = Math.Abs(s);

            return TestedDelta;
        }

        public void Mutate(int range, double prob)
        {
            for (int i = 0; i < genes.Length; ++i)
            {
                if (rand.NextDouble() <= prob)
                    genes[i] += rand.Next(-range, range + 1);
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("[ ");
            for (int i = 0; i < genes.Length; ++i)
            {
                sb.Append(genes[i]);
                if (i != genes.Length - 1)
                    sb.Append(", ");
            }
            sb.Append(" ]");
            return sb.ToString();
        }

        public void CheckEquation(int[] factors)
        {
            StringBuilder sb = new StringBuilder();

            if (factors.Length - 1 != genes.Length)
                throw new ArgumentException(string.Format("{0} factors, {1} genes", factors.Length, genes.Length));
            int s = factors[0];
            sb.Append(s);
            sb.Append(" + ");
            for (int i = 0; i < genes.Length; ++i)
            {
                sb.Append(factors[i + 1]).Append("*").Append(genes[i]);
                s += factors[i + 1] * genes[i];
                if (i < genes.Length - 1)
                    sb.Append(" + ");
            }
            sb.Append(" = ").Append(s);
            Console.WriteLine(sb.ToString());
        }
    }

    public class DiophantineSelection
    {
        private List<GenotypeDiophantine> population;
        private int initialPop;
        private double fitnessCutoffRatio;
        private int procreationsPerGenotype;
        private int mutationRange;
        private double mutationProbability;
        
        public DiophantineSelection(int initialPop, double fitnessCutoffRatio, int procreationsPerGenotype,
                                    int mutationRange, double mutationProbability)
        {
            this.initialPop = initialPop;
            this.fitnessCutoffRatio = fitnessCutoffRatio;
            this.procreationsPerGenotype = procreationsPerGenotype;
            this.mutationRange = mutationRange;
            this.mutationProbability = mutationProbability;
        }

        private void initPopulation(int genesCount, int initGenesRange)
        {
            population = new List<GenotypeDiophantine>();
            int[] genes = new int[genesCount];
            for (int i = 0; i < initialPop; ++i)
            {
                for (int j = 0; j < genesCount; ++j)
                    genes[j] = GenotypeDiophantine.rand.Next(-initGenesRange, initGenesRange+1);
                population.Add(new GenotypeDiophantine(genes));
            }
        }

        public GenotypeDiophantine Solve(int[] factors, int initGenesRange, int maxGenerations)
        {
            initPopulation(factors.Length-1, initGenesRange);
            List<GenotypeDiophantine> generationSelected = new List<GenotypeDiophantine>();

            for (int gi = 0; gi < maxGenerations; ++gi)
            {
                int maxDelta = int.MinValue;
                int minDelta = int.MaxValue;

                for (int pi = 0; pi < population.Count; ++pi)
                {
                    int delta = population[pi].TestDiophantineEquation(factors);
                    if (delta == 0)
                        return population[pi]; // We've got a winner!

                    if (delta > maxDelta)
                        maxDelta = delta;
                    if (delta < minDelta)
                        minDelta = delta;
                }


                int cutoffDelta = (int)(minDelta + (maxDelta - minDelta) * fitnessCutoffRatio);

                Console.WriteLine(string.Format("Generation {0}: population = {1}; delta = ({2}, {3})", gi, population.Count, minDelta, maxDelta));

                generationSelected.Clear();
                for (int pi = 0; pi < population.Count; ++pi)
                {
                    if (population[pi].TestedDelta <= cutoffDelta)
                    {
                        generationSelected.Add(population[pi]);
                        //Console.Write(population[pi].TestedDelta);
                        //Console.Write(" ");
                    }
                }
                //Console.Write("\n");

                // procreation
                population.Clear();

                for (int si = 0; si < generationSelected.Count; ++si)
                {
                    for (int proci = 0; proci < procreationsPerGenotype; ++proci)
                    {
                        int partner = si;
                        while (partner == si)
                            partner = GenotypeDiophantine.rand.Next(generationSelected.Count);

                        GenotypeDiophantine child = new GenotypeDiophantine(generationSelected[si], generationSelected[partner]);
                        child.Mutate(mutationRange, mutationProbability);
                        population.Add(child);
                    }
                }
            }
            return null;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            int[] factors = new int[5] { -37, 4, 2, 8, -5 };

            DiophantineSelection selection = new DiophantineSelection(
                initialPop: 10,
                fitnessCutoffRatio: 0.5,
                procreationsPerGenotype: 3,
                mutationRange: 5,
                mutationProbability: 0.25);

            GenotypeDiophantine solution = selection.Solve(factors, initGenesRange:5, maxGenerations:10);
            
            if (solution != null)
            {
                Console.WriteLine("Solution found: " + solution);
                solution.CheckEquation(factors);
            }
            else
            {
                Console.WriteLine("No solution found");
            }
        }
    }
}
