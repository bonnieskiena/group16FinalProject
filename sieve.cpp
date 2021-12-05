// C++ program for implementation of Sieve of Atkin
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <omp.h>

using namespace std;
// #define uns64 unsigned long long
// Prints all prime numbers smaller

int singleSieveBlock(int k, int S, int n, vector<int> &primes)
{
    vector<char> block(S);
    int result = 0;
    fill(block.begin(), block.end(), true);
    int start = k * S;

    for (int p : primes)
    {
        int start_idx = (start + p - 1) / p;
        int j = max(start_idx, p) * p - start;
        for (; j < S; j += p)
            block[j] = false;
    }
    if (k == 0)
        block[0] = block[1] = false;

    for (int i = 0; i < S && start + i <= n; i++)
    {
        if (block[i])
            result++;
    }
    return result;
    
	 
}

int SegmentedSieveOfErastothenes(int n, int S)

{
    double start, end;
    start = omp_get_wtime();

    //precompute first sqrt(n) primes .
    vector<int> primes;
    int nsqrt = sqrt(n);
    vector<char> is_prime(nsqrt + 2, true);
   // #pragma omp parallel for 
    for (int i = 2; i <= nsqrt; i++)
    {
        if (is_prime[i])
        {
            primes.push_back(i);
            for (int j = i * i; j <= nsqrt; j += i)
                is_prime[j] = false;
        }
    }

    int result = 0;

     #pragma omp parallel for reduction(+:result) shared(primes, S, n) 
    for (int k = 0; k <= n/S; k++)
    {
        result += singleSieveBlock(k, S, n, primes);
    }
    end = omp_get_wtime();
    double time_taken = double(end - start);
    cout << "Time taken by program is : " << fixed << time_taken << setprecision(5) << endl;
    return result;
}

int main(int argc, char **argv)
{
    const int S = 131072;   //10000;
    int n = atoll(argv[1]); // n is the number of primes to be printed
    int primes = SegmentedSieveOfErastothenes(n, S);
    cout << "Number of primes less than " << n << " is " << primes << endl;
    return 0;
}