#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

std::vector<int> A;
std::vector<int> B;
const int N = 30000000;
int i;
int localMax;
int total = 0;
void test2sections(); void test4sections();
int main()
{
    setlocale(LC_ALL, "");
    
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        A.push_back(rand() % 100);
        B.push_back(rand() % 100);
    }
    test2sections();

    std::cout << total << std::endl;
    std::cout << "runtime = " << clock() / 1000.0 << std::endl;

}
void test2sections()
{
    #pragma omp parallel sections shared(A, B, total) private(i, localMax)
    {
        #pragma omp section
        {
  
            for (i = 0; i <= N / 2; i++)
            {
                
                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
             //   std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }
        #pragma omp section
        {
            for (i = N / 2 + 1; i < N; i++)
            {

                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
              //  std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }


    }
}
void test4sections()
{
    //секции работают медленее так как на момент входа в параллельную область программа точно не знает сколько потоков будут работать
    #pragma omp parallel sections shared(A, B, total) private(i, localMax)
    {
        #pragma omp section
        {
            omp_get_num_threads();
            for (i = 0; i < N / 4; i++)
            {

                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
              //  std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }
        #pragma omp section
        {
            for (i = N / 4; i < N / 2; i++)
            {

                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
            //    std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }
         #pragma omp section
        {
            for (i = N / 2; i < N / 4 * 3; i++)
            {

                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
            //    std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }
        #pragma omp section
        {
            for (i = N / 4 * 3; i < N; i++)
            {

                localMax = std::max(A[i] + B[i], 4 * A[i] - B[i]);
             //   std::cout << "Iteration number " + std::to_string(i) + " " + std::to_string(A[i] + B[i]) + " " + std::to_string(4 * A[i] - B[i]) + "\n";
                if (localMax > 1)
                {
                    #pragma omp atomic
                    total += localMax;
                }
            }
        }
       


    }
}

// for 2 потока : 9.6
// for 4 потока: 9.3
// 2 секции: 10.3 - 10.8
// 4 секции: 10.1 - 10.6