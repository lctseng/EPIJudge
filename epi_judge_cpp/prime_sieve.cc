#include "test_framework/generic_test.h"
#include <vector>
using std::sqrt;
using std::vector;

// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
  if (n < 2)
    return {};
  // use array to mark
  // BE CAREFUL the size must +1
  vector<bool> isPrime(n / 2 + 1, true); // entry i stands for 2*i + 1
  vector<int> res(1, 2);
  int maxI = (n + 1) / 2 - 1;
  for (int i = 1; i <= maxI; i++) {
    if (isPrime[i]) {
      int val = 2 * i + 1;
      res.push_back(val);
      // sieve upto n
      for (long long int j = (long long int)val * val; j <= n; j += val) {
        if (j & 1)
          isPrime[j >> 1] = false;
      }
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                         &GeneratePrimes, DefaultComparator{}, param_names);
}
