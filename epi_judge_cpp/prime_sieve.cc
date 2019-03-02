#include "test_framework/generic_test.h"
#include <vector>
using std::sqrt;
using std::vector;

// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
  if (n < 2)
    return {};
  vector<bool> notPrime(n, false);
  vector<int> res = {2};
  for (int i = 3; i <= n; i += 2) {
    if (!notPrime[i >> 1]) {
      res.push_back(i);
      // mark future entries
      if (i <= n / i) { // Overflow check: this means n <= i * i
        int i2 = i * i;
        while (i2 <= n) {
          notPrime[i2 >> 1] = true;
          i2 += 2 * i;
        }
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
