using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Index_VS_Bruteforce
{
    class cls_RandomNumbers
    {

        private static readonly Random random = new Random();
        private static readonly object syncLock = new object();
        private static int max;
        private static int min;
        public cls_RandomNumbers(int min, int max)
        {
            cls_RandomNumbers.max = max;
            cls_RandomNumbers.min = min;
        }
        public static int RandomNumber()
        {
            lock (syncLock)
            { // synchronize
                return random.Next(min, max);
            }
        }

        public List<int> fn_UniqueRandomNumbers(int rateOfRand)
        {
            List<int> RndList = new List<int>();
            for (int i = 0; i < max; ++i)//List of accending order
            {
                RndList.Add(i);
            }
            {
                int x = 0;
                int y = 0;
                int Temp;
                for (int i = 0; i < rateOfRand; ++i) //Swap 2 random  positions with rateOfrandomization
                {
                    x = RandomNumber();
                    y = RandomNumber();
                    Temp = RndList[x];
                    RndList[x] = RndList[y];
                    RndList[y] = Temp;
                }
            }
            return RndList;
        }
    }
    class cls_Search
    {
        public double _bruteForceResult; //Holds Total Result for Bruteforce
        public double _IndexSearchResult;//Holds Total Result for IndexSearch
        int _numbeOfSteps;//loob Steps Each Algorithm take
        List<int> L_SearchList;

        public cls_Search(List<int> UniqueRandomNumbers, double BruteforceR, double IndexSearchR)//Accumilate Totlal results for percentage measure
        {
            _bruteForceResult = BruteforceR;
            _IndexSearchResult = IndexSearchR;
            _numbeOfSteps = 0;
            L_SearchList = UniqueRandomNumbers;//GetList From RandomNumbers Class
        }
        public cls_Search(List<int> UniqueRandomNumbers)
        {
            _bruteForceResult = 0;
            _IndexSearchResult = 0;
            _numbeOfSteps = 0;
            L_SearchList = UniqueRandomNumbers;//GetList From RandomNumbers Class
        }
        public void fn_BruteForce(int NumToSearch)
        {
            bool Found = false;
            _numbeOfSteps = 0;
            for (int i = 0; i < L_SearchList.Count; ++i)
            {
                _numbeOfSteps++;
                if (L_SearchList[i] == NumToSearch)
                {
                    Console.Write(_numbeOfSteps + " ");
                    _bruteForceResult += _numbeOfSteps;
                    Found = true;
                    break;
                }
            }
            if (!Found)
            {
                Console.Write("X ");
            }
        }
        public void fn_IndexSearch(int NumToSearch)
        {
            bool Found = false;
            _numbeOfSteps = 0;
            HashSet<int> hs = new HashSet<int>();//Holds The visited index <HashSet For Small Overhead searching
            Random rnd = new Random();
            for (int i = 0; _numbeOfSteps < L_SearchList.Count; )
            {
                _numbeOfSteps++;
                while (hs.Contains(i))//if index in visited state
                {
                    i = rnd.Next(0, L_SearchList.Count);
                }
                hs.Add(i); //add index to visited state

                i = L_SearchList[i];
                if (NumToSearch == i)
                {
                    Console.WriteLine(_numbeOfSteps + " ");
                    Found = true;
                    _IndexSearchResult += _numbeOfSteps;
                    break;
                }
            }
            if (!Found)
            {
                Console.WriteLine("X  ");
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            const int min = 0;
            const int max = 100;
            cls_Search S;

            cls_RandomNumbers rnd = new cls_RandomNumbers(min, max);
            S = new cls_Search(rnd.fn_UniqueRandomNumbers(max));

            int x = 0, y = 0; //X for Bruteforce Score Y for SearchIndex Score

            for (int j = 0; j < 100; ++j)//to Calculate the percentage each algorithm outperform the other
            {
                rnd = new cls_RandomNumbers(min, max);
                S = new cls_Search(rnd.fn_UniqueRandomNumbers(max));

                for (int i = min; i < max; ++i)
                {
                    Console.Write(i + " ");
                    S.fn_BruteForce(i);
                    S.fn_IndexSearch(i);
                }
                if (S._bruteForceResult <= S._IndexSearchResult)
                {
                    x++;
                }
                else
                {
                    y++;
                }
            }


            Console.WriteLine(x + " " + y);//X+Y =100
            Console.ReadKey();
        }
    }
}
